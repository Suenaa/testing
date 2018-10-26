#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Storage.hpp"
#include "Path.hpp"

using namespace std;

// instance of Storage
shared_ptr<Storage> Storage::m_instance = nullptr;

/**
*   default constructor
*/
Storage::Storage() {
    m_dirty = false;
    this->readFromFile();
}

/**
*   remove the double quatations in the token
*   @param tokens the source tokens
*/
static void removeQuatation(vector<string> &tokens) {
    for (auto it = tokens.begin(); it != tokens.end(); it++) {
        if ((*it)[0] == '\"') {
            it->erase(it->begin());
        }
        if ((*it)[(*it).length() - 1] == '\"') {
            it->erase((*it).length() - 1);
        }
    }
}

/**
*   split a string into vector using the given delim
*   @param t_str the source string
*   @param t_delim the delim of the string
*   @return a vector of string
*/
static vector<string> split(const string &t_str, char t_delim) {
    stringstream ss(t_str);
    string item;
    vector<string> tokens;
    while (getline(ss, item, t_delim)) {
        tokens.push_back(item);
    }
    removeQuatation(tokens);
    return tokens;
}

vector<string> stringToVector(string t_str) {
    stringstream ss(t_str);
    string item;
    vector<string> participatorlist;
    while (getline(ss, item, '&')) {
        participatorlist.push_back(item);
    }
    return participatorlist;
}

/**
*   read file content into memory
*   @return if success, true will be returned
*/
bool Storage::readFromFile(void) {
    ifstream users_ifs(Path::userPath, ios::in);
    ifstream meetings_ifs(Path::meetingPath, ios::in);
    if (!(users_ifs.good() && meetings_ifs.good())) {
        return false;
    }
    string line;
    // skip the first line
    // getline(users_ifs, line);
    while (getline(users_ifs, line)) {
        // remove the first and last character
        if (line.length() < 1) {
            continue;
        }
        string real_line(line.begin() + 1,
                              line.begin() + line.length() - 1);
        vector<string> data = split(real_line, ',');
        if (data.size() != 4) {
            continue;
        }
        m_userList.push_back({data[0], data[1], data[2], data[3]});
    }
    users_ifs.close();
    // getline(users_ifs, line);
    while (getline(meetings_ifs, line)) {
        // remove the first and last character
        if (line.length() < 1) {
            continue;
        }
        string real_line(line.begin() + 1,
                              line.begin() + line.length() - 1);
        vector<string> data = split(real_line, ',');
        if (data.size() != 5) {
            continue;
        }
        m_meetingList.push_back(
            {data[0], stringToVector(data[1]), data[2], data[3], data[4]});
    }
    return true;
}

/**
*   join factors to a string
*   @param t_factors the string factors
*   @return a csv string
*/
string generate_csv_line(vector<string> t_factors) {
    string ret;
    bool first = true;
    for (string &each : t_factors) {
        if (!first) {
            ret += ",";
        }
        ret += '\"';
        ret += each;
        ret += '\"';
        first = false;
    }
    return ret;
}

/**
* convert the participatorlist to string
* @param the source participatorlist
* @return the result string
*/
string vectorToString(vector<string> &participatorlist) {
    string result = "";
    bool isFirstItem = true;
    for (auto it = participatorlist.begin(); it != participatorlist.end();
         it++) {
        if (isFirstItem) {
            isFirstItem = false;
        } else {
            result += '&';
        }
        result += *it;
    }
    return result;
}

/**
*   write file content from memory
*   @return if success, true will be returned
*/
bool Storage::writeToFile(void) {
    if (!m_dirty) {
        return false;
    }
    ofstream users_ifs(Path::userPath, ios::out);
    ofstream meetings_ifs(Path::meetingPath, ios::out);
    for (User &each : m_userList) {
        users_ifs << generate_csv_line({each.getName(), each.getPassword(),
                                        each.getEmail(), each.getPhone()})
                  << endl;
    }
    for (Meeting &each : m_meetingList) {
        auto participatorlist = each.getParticipator();
        meetings_ifs << generate_csv_line(
                            {each.getSponsor(),
                             vectorToString(participatorlist),
                             Date::dateToString(each.getStartDate()),
                             Date::dateToString(each.getEndDate()),
                             each.getTitle()}) << endl;
    }
    meetings_ifs.close();
    users_ifs.close();
    m_dirty = false;
    return true;
}

/**
* get Instance of storage
* @return the pointer of the instance
*/
shared_ptr<Storage> Storage::getInstance(void) {
    if (m_instance == nullptr) {
        m_instance = shared_ptr<Storage>(new Storage);
    }
    return m_instance;
}

/**
*   destructor
*/
Storage::~Storage() {
    // cout << "delete storage" << endl;
    if (this->m_dirty) {
        this->writeToFile();
    }
}

// CRUD for User & Meeting
// using C++11 Function Template and Lambda Expressions

/**
* create a user
* @param a user object
*/
void Storage::createUser(const User &t_user) {
    m_userList.push_back(t_user);
    m_dirty = true;
    this->sync();
}

/**
* query users
* @param a lambda function as the filter
* @return a list of fitted users
*/
list<User> Storage::queryUser(
    function<bool(const User &)> filter) const {
    list<User> fitted_users;
    for (auto &each : m_userList) {
        if (filter(each)) {
            fitted_users.push_back(each);
        }
    }
    return fitted_users;
}

/**
* update users
* @param a lambda function as the filter
* @param a lambda function as the method to update the user
* @return the number of updated users
*/
int Storage::updateUser(function<bool(const User &)> filter,
                        function<void(User &)> switcher) {
    int updated_count = 0;
    for (auto &each : m_userList) {
        if (filter(each)) {
            switcher(each);
            updated_count++;
            m_dirty = true;
            this->sync();
        }
    }
    return updated_count;
}

/**
* delete users
* @param a lambda function as the filter
* @return the number of deleted users
*/
int Storage::deleteUser(function<bool(const User &)> filter) {
    int deleted_count = 0;
    for (auto it = m_userList.begin(); it != m_userList.end();) {
        if (filter(*it)) {
            it = m_userList.erase(it);
            deleted_count++;
            m_dirty = true;
            this->sync();
        } else {
            it++;
        }
    }
    return deleted_count;
}

/**
* create a meeting
* @param a meeting object
*/
void Storage::createMeeting(const Meeting &t_meeting) {
    m_meetingList.push_back(t_meeting);
    m_dirty = true;
    this->sync();
}

/**
* query meetings
* @param a lambda function as the filter
* @return a list of fitted meetings
*/
list<Meeting> Storage::queryMeeting(
    function<bool(const Meeting &)> filter) const {
    list<Meeting> fitted_meetings;
    for (auto &each : m_meetingList) {
        if (filter(each)) {
            fitted_meetings.push_back(each);
        }
    }
    return fitted_meetings;
}

/**
* update meetings
* @param a lambda function as the filter
* @param a lambda function as the method to update the meeting
* @return the number of updated meetings
*/
int Storage::updateMeeting(function<bool(const Meeting &)> filter,
                           function<void(Meeting &)> switcher) {
    int updated_count = 0;
    for (auto &each : m_meetingList) {
        if (filter(each)) {
            switcher(each);
            updated_count++;
            m_dirty = true;
            this->sync();
        }
    }
    return updated_count;
}

/**
* delete meetings
* @param a lambda function as the filter
* @return the number of deleted meetings
*/
int Storage::deleteMeeting(function<bool(const Meeting &)> filter) {
    int deleted_count = 0;
    for (auto it = m_meetingList.begin(); it != m_meetingList.end();) {
        if (filter(*it)) {
            it = m_meetingList.erase(it);
            deleted_count++;
            m_dirty = true;
            this->sync();
        } 
        else {
            it++;
        }
    }
    return deleted_count;
}

/**
* sync with the file
*/
bool Storage::sync(void) {
    return writeToFile();
}