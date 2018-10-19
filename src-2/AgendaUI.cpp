#include "AgendaUI.hpp"
#include <iostream>

using namespace std;

AgendaUI::AgendaUI() {
	startAgenda();
	}

void AgendaUI::OperationLoop(void) {
	for (int i = 0; i < 23; i++) 
		cout << "-";
	cout << " Agenda ";
	for (int i = 0; i < 23; i++)
		cout << "-";
	cout << endl;
	cout << "Action :" << endl;
	cout << "l   - log in Agenda by user name and password" << endl;
	cout << "r   - register an Agenda account" << endl;
	cout << "q   - quit Agenda" << endl;
	for (int i = 0; i < 54; i++)
		cout << "-";
	cout << endl;
	cout << "Agenda : ~$ ";
	string operation;
	cin >> operation;
	if (operation == "l")
		userLogIn();
	else if (operation == "r")
		userRegister();
	else if (operation == "q")
		quitAgenda();
	else {
		cout << "error!" << endl;
		OperationLoop();
	}
}

void AgendaUI::startAgenda(void) {
	m_agendaService.startAgenda();
}

string AgendaUI::getOperation() {
	for (int i = 0; i < 23; i++) 
		cout << "-";
	cout << " Agenda ";
	for (int i = 0; i < 23; i++)
		cout << "-";
	cout << endl;
	cout << "Action :" << endl;
	cout << "o   - log out Agenda" << endl;
	cout << "dc  - delete Agenda account" << endl;
	cout << "lu  - list all Agenda user" << endl;
	cout << "cm  - create a meeting" << endl;
	cout << "la  - list all meetings" << endl;
	cout << "las - list all sponsor meetings" << endl;
	cout << "lap - list all participate meetings" << endl;
	cout << "qm  - query meeting by title" << endl;
	cout << "qt  - query meeting by time interval" << endl;
	cout << "da  - delete meeting by title" << endl;
	for (int i = 0; i < 54; i++)
		cout << "-";
	cout << endl << endl;
	cout << "Agenda@" << m_userName << " : # ";
	string operation;
	cin >> operation;
	return operation;
}

bool AgendaUI::executeOperation(string t_operation) {
	if (t_operation == "o") {
		userLogOut();
		return false;
	}
	else if (t_operation == "dc") {
		deleteUser();
		return false;
	}
	else if (t_operation == "lu") {
		listAllUsers();
		return true;
	}
	else if (t_operation == "cm") {
		createMeeting();
		return true;
	}
	else if (t_operation == "la") {
		listAllMeetings();
		return true;
	}
	else if (t_operation == "las") {
		listAllSponsorMeetings();
		return true;
	}
	else if (t_operation == "lap") {
		listAllParticipateMeetings();
		return true;
	}
	else if (t_operation == "qm") {
		queryMeetingByTitle();
		return true;
	}
	else if (t_operation == "qt") {
		queryMeetingByTimeInterval();
		return true;
	}
	else if (t_operation == "dm") {
		deleteMeetingByTitle();
		return true;
	}
	else if (t_operation == "da") {
		deleteAllMeetings();
		return true;
	}
//	else if (t_operation == "rp") {

//	}
	return true;
}

void AgendaUI::userLogIn(void) {
	cout << "[Log in] [user name] [password]" << endl;
	cout << "[log in] ";
	string userName, userPassword;
	cin>> userName >> userPassword;
	if(m_agendaService.userLogIn(userName, userPassword)) {
		m_userName = userName;
		m_userPassword = userPassword;
		cout << "[log in] succeed!" << endl;
		while(executeOperation(getOperation()));
	}
	else {
		cout << "[error] log in fail!" << endl << endl;
		OperationLoop();
	}
}

void AgendaUI::userRegister(void) {
	string Name, password, email, phone;
	cout << "[register] [user name] [password] [email] [phone]" << endl;
	cout << "[register] ";
	cin >> Name >> password >> email >> phone;
	if(m_agendaService.userRegister(Name, password, email, phone)) {
		cout << "[register] succeed!" << endl;
		while(executeOperation(getOperation()));
	}
	else {
		cout << "[error] register fail!" << endl;
		OperationLoop();
	}
}

void AgendaUI::quitAgenda(void) {
	m_agendaService.quitAgenda();
	cout << "[quit] succeed!" << endl;
}

void AgendaUI::userLogOut(void) {
	m_agendaService.quitAgenda();
	OperationLoop();
}

void AgendaUI::deleteUser(void) {
	if(m_agendaService.deleteUser(m_userName, m_userPassword)) {
		m_agendaService.quitAgenda();
		cout << "[delete agenda account] succeed!" << endl;
	}
	else {
		cout << "[error] delete agenda account fail!" << endl;
		//executeOperation(getOperation());
	}
}

void AgendaUI::listAllUsers(void) {
	list<User> listUsers = m_agendaService.listAllUsers();
	cout << "[list all users]" << endl << endl;
	cout << "name                email               phone" << endl;
	for (auto i = listUsers.begin(); i != listUsers.end(); i++) {
		cout << (*i).getName();
		cout.width(20 - (*i).getName().size());
		cout << (*i).getEmail();
		cout.width(20 - (*i).getEmail().size());
		cout << (*i).getPhone() << endl;
	}
}

void AgendaUI::createMeeting() {
	int num;
	string title, startDate, endDate;
	vector<string> participator;
	cout << "[create meeting] [the number of participators]" << endl;
	cout << "[create meeting] ";
	cin >> num;
	for (int i = 0; i < num; i++) {
		cout << "[create meeting] [please enter the participator " << i + 1 << " ]" << endl;
		cout << "[create meeting] ";
		cin >> participator[i];
	}
	cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" 
	<< endl;
	cout << "[create meeting] ";
	cin >> title >> startDate >> endDate;
	if (m_agendaService.createMeeting(m_userName, title, startDate, endDate, participator)) {
		cout << "[create meeting] succeed!" << endl;
	}
	else {
		cout << "[create meeting] error!" << endl;
		//executeOperation(getOperation());
	}
}

void AgendaUI::listAllMeetings() {
	list<Meeting> Meetings = m_agendaService.listAllMeetings(m_userName);
	cout << "[list all meetings]" << endl << endl;
	printMeetings(Meetings);
}

void AgendaUI::listAllSponsorMeetings(void) {
	list<Meeting> sponsorMeetings = m_agendaService.listAllSponsorMeetings(m_userName);
	cout << "[list all sponsor meetings]" << endl << endl;
	printMeetings(sponsorMeetings);
}

void AgendaUI::listAllParticipateMeetings() {
	list<Meeting> participateMeetings = m_agendaService.listAllParticipateMeetings(m_userName);
	cout << "[list all participator meetings]" << endl << endl;
	printMeetings(participateMeetings);
}

void AgendaUI::queryMeetingByTitle() {
	string title;
	cout << "[query meeting] [title]:" << endl << "[query meeting] ";
	cin >> title;
	list<Meeting> titleMeeting = m_agendaService.meetingQuery(m_userName, title);
	printMeetings(titleMeeting);
}

void AgendaUI::queryMeetingByTimeInterval() {
	string start, end;
	cout << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
	cout << "[query meetings] ";
	cin >> start >> end;
	list<Meeting> timeMeeting = m_agendaService.meetingQuery(m_userName, start, end);
	printMeetings(timeMeeting);
}

void AgendaUI::deleteMeetingByTitle() {
	cout << endl << "[delete meeting] [title]" << endl;
	cout << "[delete meeting] ";
	string title;
	cin >> title;
	if (m_agendaService.deleteMeeting(m_userName, title)) {
		cout << endl << "[delete meeeting] succeed!" << endl;
	}
	else {
		cout << endl << "[error] delete meeting fail!" << endl;
		//executeOperation(getOperation());
	}
}

void AgendaUI::deleteAllMeetings() {
	if (m_agendaService.deleteAllMeetings(m_userName)) {
		cout << endl << "[delete all meetings] succeed!" << endl;
	}
	else {
		cout << endl << "[error] delete all meetings fail!" << endl;
		//executeOperation(getOperation());
	}
}

void AgendaUI::printMeetings(list<Meeting> t_meetings) {
	list<Meeting> Meeting = t_meetings;
	cout << "[query meetings]" << endl << endl;
	cout << "title          sponsor        start time          ";
	cout << "end time            participators" << endl;
	for (auto i = Meeting.begin(); i != Meeting.end(); i++) {
		cout << (*i).getTitle();
		cout.width(15 - (*i).getTitle().size());
		cout << (*i).getSponsor();
		cout.width(15 - (*i).getSponsor().size());
		cout << (*i).getStartDate().getYear() << "-" << (*i).getStartDate().getMonth() 
		<< "-" << (*i).getStartDate().getDay() << "/" << (*i).getStartDate().getHour() 
		<< ":" << (*i).getStartDate().getMinute();
		cout.width(4);
		cout << (*i).getEndDate().getYear() << "-" << (*i).getEndDate().getMonth() 
		<< "-" << (*i).getEndDate().getDay() << "/" << (*i).getEndDate().getHour() 
		<< ":" << (*i).getEndDate().getMinute();
		cout.width(4);
		vector<string> participator = (*i).getParticipator();
		cout << participator[0];
		for (int j = 1; j < participator.size(); j++) 
			cout << "," << participator[j];
		cout << endl;
	}
}