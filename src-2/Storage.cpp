#include <iostream>
#include <fstream>
#include <functional>
#include <list>
#include <memory>
#include <vector>
#include <string>
#include "Path.hpp"
#include "Date.hpp"
#include "Storage.hpp"

using namespace std;

Storage::Storage() {
	readFromFile();
}

bool Storage::readFromFile(void) {
	ifstream inUser(Path::userPath);
	ifstream inMeeting(Path::meetingPath);
	if (!inUser.is_open() || !inMeeting.is_open())
		return false;
	string str, value, name, password, email, phone;
	int count = 1;
	while(!inUser.eof()) {
		getline(inUser, value);
		auto sptr = value.c_str();
		while (*sptr != '\0') {
			if (*sptr == '"') {
				sptr++;
			}
			else if (*sptr == ',') {
				if (count == 1) {
					name = str;
					count = 2;
				}
				else if (count == 2) {
					password= str;
					count = 3;
				}
				else if (count == 3) {
					email = str;
					count = 1;
				}
				sptr++;
			}
			else {
				str.push_back(*sptr);
			}
		}
		phone = str;
		User temp = User(name, password, email, phone);
		m_userList.push_back(temp);
		str = "";
	}
	count = 1;
	string sponsor, participators, startD, endD, title;
	Date start;
	Date end;
	Meeting tempM;
	vector<string> v;
	while(!inMeeting.eof()) {
		getline(inMeeting, value);
		auto sptr = value.c_str();
		while (*sptr != '\0') {
			if (*sptr == '"') {
				sptr++;
			}
			else if (*sptr == ',') {
				if (count == 1) {
					sponsor = str;
					count = 2;
				}
				else if (count == 2) {
					participators = str;
					string s;
					auto pptr = participators.c_str();
					while(*pptr != '\0') {
						if (*pptr == '&') {
							v.push_back(s);
							pptr++;
							s = "";
						}
						else {
							s.push_back(*pptr);
							pptr++;
						}
					}
					v.push_back(s);
					s = "";
					count = 3;
				}
				else if (count == 3) {
					startD = str;
					start = Date(startD);
					count = 1;
				}
				else if (count == 4) {
					endD = str;
					end = Date(endD);
					count = 1;
				}
				sptr++;
			}
			else {
				str.push_back(*sptr);
			}
		}
		title = str;
		tempM = Meeting(sponsor, v, start, end, title);
		m_meetingList.push_back(tempM);
		str = "";
	}
	return true; 
}

bool Storage::writeToFile(void) {
	ofstream oUser(Path::userPath);
	ofstream oMeeting(Path::meetingPath);
	if (!oUser.is_open() || !oMeeting.is_open())
		return false;
	for (auto i = m_userList.begin(); i != m_userList.end(); i++) {
		oUser << "\"" <<  (*i).getName() << "\",\"" 
		<< (*i).getPassword() << "\",\"" 
		<< (*i).getEmail() << "\",\"" 
		<< (*i).getPhone() << "\"" << endl;
	}
	for (auto i = m_meetingList.begin(); i != m_meetingList.end(); i++) {
		oMeeting << "\"" << (*i).getSponsor() << "\",\"";
		for (auto j = (*i).getParticipator().begin(); j != (*i).getParticipator().end() - 1; 
			j++) {
			oMeeting << *j << "&";
		}
		auto k = (*i).getParticipator().end();
		oMeeting << *k << "\",\"" << (*i).getStartDate().dateToString((*i).getStartDate())
		<< "\",\"" << (*i).getEndDate().dateToString((*i).getEndDate()) 
		<< "\",\"" << (*i).getTitle() << "\"" << endl;
	}
	oUser.close();
	oMeeting.close();
	return true;
}

shared_ptr<Storage> Storage::getInstance(void) {
	if (m_instance == nullptr) {
		shared_ptr<Storage> t(new Storage);
		m_instance = t;
	}
	return m_instance;
}

Storage::~Storage() {
	if (m_dirty) {
		sync();
	}
	m_instance = nullptr;
}

void Storage::createUser(const User & t_user) {
	m_userList.push_back(t_user);
}

list<User> Storage::queryUser(function<bool(const User &) > filter) const {
	list<User> queryResult;
	for (auto i = m_userList.begin(); i!= m_userList.end(); i++) {
		if (filter(*i)) {
			queryResult.push_back(*i);
		}
	}
	return queryResult;
}

int Storage::updateUser(function<bool(const User &)> filter,
            function<void(User &)> switcher) {
	int count = 0;
	for (auto i = m_userList.begin(); i != m_userList.end(); i++) {
		if (filter(*i)) {
			switcher(*i);
			count++;
		}
	}
	return count;
}
//
int Storage::deleteUser(function<bool(const User &)> filter) {
	int count = 0;
	for (auto i = m_userList.begin(); i != m_userList.end();) {
		if (filter(*i++)) {
			i = m_userList.erase(i);
			count++;
		}
		else {
			i++;
		}
	}
	return count;
}

void Storage::createMeeting(const Meeting & t_meeting) {
	m_meetingList.push_back(t_meeting);
}

list<Meeting> Storage::queryMeeting(function<bool(const Meeting &)> filter) const {
	list<Meeting> queryResult;
	for(auto i = m_meetingList.begin(); i != m_meetingList.end(); i++) {
		if (filter(*i))
			queryResult.push_back(*i);
	}
	return queryResult;
}

int Storage::updateMeeting(function<bool(const Meeting &)> filter,
	function<void(Meeting &)> switcher) {
	int count = 0;
	for (auto i = m_meetingList.begin(); i != m_meetingList.end(); i++) {
		if (filter(*i)) {
			switcher(*i);
			count++;
		}
	}
	return count;
}
//
int Storage::deleteMeeting(function<bool(const Meeting &)> filter) {
	int count = 0;
	for (auto i = m_meetingList.begin(); i != m_meetingList.end();) {
		if (filter(*i++)) {
			i = m_meetingList.erase(i);
			count++;
		}
		else {
			i++;
		}
	}
	return count;
}

bool Storage::sync(void) {
	return writeToFile();
}

shared_ptr<Storage> Storage::m_instance = nullptr;