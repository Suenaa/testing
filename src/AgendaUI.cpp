#include "AgendaUI.hpp"
#include "AgendaService.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

using std::setw;
using namespace std;

AgendaUI::AgendaUI() {
	m_userName = "";
	m_userPassword = "";
	startAgenda();
}

void AgendaUI::OperationLoop(void) {
	while (true) {
		for (int i = 0; i < 23; i++) 
			cout << "-";
		cout << " Agenda ";
		for (int i = 0; i < 23; i++)
			cout << "-";
		cout << endl;
		cout << "Action :" << endl;
		if (m_userName == "") {
			cout << "l   - log in Agenda by user name and password" << endl;
			cout << "r   - register an Agenda account" << endl;
			cout << "q   - quit Agenda" << endl;
		}
		else {
			cout << "o   - log out Agenda" << endl;
			cout << "dc  - delete Agenda account" << endl;
			cout << "lu  - list all Agenda user" << endl;
			cout << "cm  - create a meeting" << endl;
			cout << "la  - list all meetings" << endl;
			cout << "las - list all sponsor meetings" << endl;
			cout << "lap - list all participate meetings" << endl;
			cout << "qm  - query meeting by title" << endl;
			cout << "qt  - query meeting by time interval" << endl;
			cout << "dm  - delete meeting by title" << endl;
			cout << "da  - delete all meetings" << endl;
		}
		for (int i = 0; i < 54; i++)
			cout << "-";
		cout << endl;
		if (m_userName == "") {
			cout << "Agenda : ~$ ";
		}
		else {
			cout << "Agenda@" << m_userName << " :~# ";
		}
		// executeOperation();
		if (!executeOperation(getOperation()))
			break;
	}
}

void AgendaUI::startAgenda(void) {
	m_agendaService.startAgenda();
}

string AgendaUI::getOperation() {
	string operation;
	cin >> operation;
	return operation;
}

bool AgendaUI::executeOperation(string t_operation) {
	bool flag = true;
	if (m_userName == "") {
		flag = false;
	}
	if (t_operation == "l" && flag == false) {
		userLogIn();
		return true;
	}
	else if (t_operation == "r" && flag == false) {
		userRegister();
		return true;
	}
	else if (t_operation == "q" && flag == false) {
		m_agendaService.quitAgenda();
        	return false;
	}
	else if (t_operation == "o" && flag) {
		userLogOut();
		return true;
	}
	else if (t_operation == "dc" && flag) {
		deleteUser();
		return true;
	}
	else if (t_operation == "lu" && flag) {
		listAllUsers();
		return true;
	}
	else if (t_operation == "cm" && flag) {
		createMeeting();
		return true;
	}
	else if (t_operation == "la" && flag) {
		listAllMeetings();
		return true;
	}
	else if (t_operation == "las" && flag) {
		listAllSponsorMeetings();
		return true;
	}
	else if (t_operation == "lap" && flag) {
		listAllParticipateMeetings();
		return true;
	}
	else if (t_operation == "qm" && flag) {
		queryMeetingByTitle();
		return true;
	}
	else if (t_operation == "qt" && flag) {
		queryMeetingByTimeInterval();
		return true;
	}
	else if (t_operation == "dm" && flag) {
		deleteMeetingByTitle();
		return true;
	}
	else if (t_operation == "da" && flag) {
		deleteAllMeetings();
		return true;
	}
	else {
		cout << "Please enter correct command" << endl;
	}
//	else if (t_operation == "rp") {

//	}
	return true;
}

void AgendaUI::userLogIn(void) {
	cout << endl;
	cout << "[Log in] [user name] [password]" << endl;
	cout << "[log in] ";
	string userName, userPassword;
	cin>> userName >> userPassword;
	if(m_agendaService.userLogIn(userName, userPassword)) {
		m_userName = userName;
		m_userPassword = userPassword;
		cout << "[log in] succeed!" << endl;
	}
	else {
		cout << "[error] log in fail!" << endl << endl;
		m_userName = "";
		m_userPassword = "";
	}
	cout << endl;
}

void AgendaUI::userRegister(void) {
	cout << endl;
	string Name, password, email, phone;
	cout << "[register] [user name] [password] [email] [phone]" << endl;
	cout << "[register] ";
	cin >> Name >> password >> email >> phone;
	if(m_agendaService.userRegister(Name, password, email, phone)) {
		cout << "[register] succeed!" << endl;
	}
	else {
		cout << "[error] register fail!" << endl;
	}
	cout << endl;
}

void AgendaUI::quitAgenda(void) {
	m_agendaService.quitAgenda();
}

void AgendaUI::userLogOut(void) {
	m_userName = "";
	m_userPassword = "";
}

void AgendaUI::deleteUser(void) {
	cout << endl;
	if(m_agendaService.deleteUser(m_userName, m_userPassword)) {
		m_userName = "";
		m_userPassword = "";
		cout << "[delete agenda account] succeed!" << endl;
	}
	else {
		cout << "[error] delete agenda account fail!" << endl;
		//executeOperation(getOperation());
	}
	cout << endl;
}

void AgendaUI::listAllUsers(void) {
	cout << endl;
	list<User> listUsers = m_agendaService.listAllUsers();
	cout << "[list all users]" << endl << endl;
	cout << setiosflags(ios::left);
	cout << setw(20) << "name";
	cout << setw(30) << "email";
	cout << "phone" << endl;
	for (auto i = listUsers.begin(); i != listUsers.end(); i++) {
		cout << setw(20) << (*i).getName();
		cout << setw(30) << (*i).getEmail();
		cout << (*i).getPhone() << endl;
	}
	cout << endl;
}

void AgendaUI::createMeeting() {
    cout << endl;
    cout << "[create meeting] [the number of participators]" << endl
         << "[create meeting] ";
    int num;
    cin >> num;
    if (num == 0) {
        cout << "[create meeting] the number of participators can't be zero" << endl;
        return;
    }
    vector<string> l;
    string temp;
    for (int i = 1; i <= num; i++) {
        cout << "[create meeting] [please enter the participator " <<
                    i << " ]" << endl
             << "[create meeting] ";
        cin >> temp;
        l.push_back(temp);
    }
    cout << "[create meeting] [title]"
         << "[start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]"
         << endl;
    cout << "[create meeting] ";
    string sTime, eTime, title;
    cin >> title >> sTime >> eTime;
    if (m_agendaService.createMeeting(m_userName, title, sTime, eTime, l)) {
        cout << "[create meeting] succeed!" << endl;
    } 
    else {
        cout << "[create meeting] error!" << endl;
    }
    cout << endl;
}

void AgendaUI::listAllMeetings() {
	cout << endl;
	list<Meeting> Meetings = m_agendaService.listAllMeetings(m_userName);
	cout << "[list all meetings]" << endl << endl;
	if (Meetings.empty()) {
		cout << "None." << endl << endl;
	}
	else {
		printMeetings(Meetings);
	}
}

void AgendaUI::listAllSponsorMeetings(void) {
	cout << endl;
	list<Meeting> sponsorMeetings = m_agendaService.listAllSponsorMeetings(m_userName);
	cout << "[list all sponsor meetings]" << endl << endl;
	if (sponsorMeetings.empty()) {
		cout << "None." << endl << endl;
	}
	else {
		printMeetings(sponsorMeetings);
	}
}

void AgendaUI::listAllParticipateMeetings() {
	cout << endl;
	list<Meeting> participateMeetings = m_agendaService.listAllParticipateMeetings(m_userName);
	cout << "[list all participator meetings]" << endl << endl;
	if (participateMeetings.empty()) {
		cout << "None." << endl << endl;;
	}
	else {
		printMeetings(participateMeetings);
	}
}

void AgendaUI::queryMeetingByTitle() {
	string title;
	cout << endl;
	cout << "[query meeting] [title]:" << endl << "[query meeting] ";
	cin >> title;
	cout << endl;
	list<Meeting> titleMeeting = m_agendaService.meetingQuery(m_userName, title);
	if (titleMeeting.empty()) {
		cout << "None." << endl << endl;
	}
	else {
		printMeetings(titleMeeting);
	}
}

void AgendaUI::queryMeetingByTimeInterval() {
	string start, end;
	cout << endl;
	cout << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
	cout << "[query meetings] ";
	cin >> start >> end;
	cout << endl;
	list<Meeting> timeMeeting = m_agendaService.meetingQuery(m_userName, start, end);
	if (timeMeeting.empty()) {
		cout << "None." << endl << endl;
	}
	else {	
		printMeetings(timeMeeting);
	}
}

void AgendaUI::deleteMeetingByTitle() {
	cout << endl;
	cout << endl << "[delete meeting] [title]" << endl;
	cout << "[delete meeting] ";
	string title;
	cin >> title;
	if (m_agendaService.deleteMeeting(m_userName, title)) {
		cout << endl << "[delete meeeting] succeed!" << endl;
	}
	else {
		cout << endl << "[error] delete meeting fail!" << endl;
	}
	cout << endl;
}

void AgendaUI::deleteAllMeetings() {
	cout << endl;
	if (m_agendaService.deleteAllMeetings(m_userName)) {
		cout << endl << "[delete all meetings] succeed!" << endl;
	}
	else {
		cout << endl << "[error] delete all meetings fail!" << endl;
	}
	cout << endl;
}

void AgendaUI::printMeetings(list<Meeting> t_meetings) {
	list<Meeting> Meeting = t_meetings;
	cout << endl;
	cout << setiosflags(ios::left);
	cout << setw(15) << "title";
	cout << setw(15) << "sponsor";
	cout << setw(18) << "start time";
	cout << setw(18) << "end time";
	cout << "participators" << endl;
	for (auto i = Meeting.begin(); i != Meeting.end(); i++) {
		cout << setw(15) << (*i).getTitle();
		cout << setw(15) << (*i).getSponsor();
		cout << setw(18) << Date::dateToString(i->getStartDate()); 
		cout << setw(18) << Date::dateToString(i->getEndDate());
		vector<string> participator = (*i).getParticipator();
		cout << participator[0];
		for (int j = 1; j < participator.size(); j++) 
			cout << "," << participator[j];
		cout << endl;
	}
	cout << endl;
}
