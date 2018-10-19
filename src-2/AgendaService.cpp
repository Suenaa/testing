#include <iostream>
#include "AgendaService.hpp"
#include "Storage.hpp"

using namespace std;

AgendaService::AgendaService() {
	startAgenda();
}

AgendaService::~AgendaService() {
	quitAgenda();
}

bool AgendaService::userLogIn(const string userName, const string password) {
	auto fun = [userName, password] (User it) {
		return (userName == it.getName() && password == it.getPassword());
	};
	if ((m_storage->queryUser(fun)).size() != 0)
		return true;
	else 
		return false;
}
/*bool AgendaService::userLogIn(const string userName, const string password) {
	list<User> result = m_storage->queryUser([userName, password] (User  it) -> bool {
		return userName == it.getName() && password == it.getPassword();
	});
	if (result.size() != 0)
		return true;
	else
		return false;
}*/

bool AgendaService::userRegister(const string userName, const string password, 
	const string email, const string phone) {
	/*list<User> t_Users = m_storage->queryUser([userName] (const User &it) -> bool {
		cout << "AgendaService userRegister filter" << endl;
		return userName == it.getName();
	});
	if (t_Users.empty()) {
		User temp(userName, password, email, phone);
		m_storage->createUser(temp);
		return true;
	}
	else {
		return false;
	}*/
	if (m_storage->queryUser([&](const User &it){
		cout << "1" << endl;
		if(userName == it.getName())
			return true;
		else
			return false;
	}).empty()) {
		User temp(userName, password, email, phone);
		m_storage->createUser(temp);
		return true;
	}
	else {
		return false;
	}
}

bool AgendaService::deleteUser(const string userName, const string password) {
	int count = m_storage->deleteUser([userName] (User it) -> bool {
		if (userName == it.getName())
			return true;
		return false;
	});
	if (count == 0)
		return false;
	else
		return true;
}

list<User> AgendaService::listAllUsers(void) const {
	list<User> result = m_storage->queryUser([] (User it) -> bool {
		return true;
	});
	return result;
}

bool AgendaService::createMeeting(const string userName, const string title,
	const string startDate, const string endDate, const vector<string> participator) {
	Date start(startDate);
	Date end(endDate);
	Meeting temp(userName, participator, start, end, title);
	if (participator.size() == 0)
		return false;
	for (int i = 0; i < participator.size(); i++) {
		list<User> t_user = m_storage->queryUser([participator, i] (User it) 
			-> bool {
				return participator[i] == it.getName();
			});
		if (t_user.size() == 0)
			return false;
	}
	list <Meeting> result = m_storage->queryMeeting([=] (Meeting it) -> bool {
		if (title == it.getTitle())
			return true;
		else if ((userName == it.getSponsor() || it.isParticipator(userName)) 
			&& ((it.getStartDate() <= start && it.getEndDate() > start)
			|| (it.getStartDate() > start && it.getStartDate() <= end)))
			return true;
		else
			return false;
	});
	if (result.size() != 0)
		return false;
	for (int i = 0; i < participator.size(); i++) {
		list<Meeting> result2 = m_storage->queryMeeting([=] 
			(Meeting it) -> bool {
				if ((participator[i] == it.getSponsor() || it.isParticipator(participator[i])) 
					&& ((it.getStartDate() <= start && it.getEndDate() > start)
					|| (it.getStartDate() > start && it.getStartDate() <= end)))
					return true;
				else
					return false;
			});
		if (result2.size() != 0)
			return false;
	}
	m_storage->createMeeting(temp);
	return true;
}

list<Meeting> AgendaService::meetingQuery(const string userName, string title) const {
	list<Meeting> result = m_storage->queryMeeting([title] (Meeting it) -> bool {
		if (title == it.getTitle())
			return true;
		else
			return false;
	});
	return result;
}

list<Meeting> AgendaService::meetingQuery(const string userName, 
	const string startDate, const string endDate) const {
	Date start(startDate);
	Date end(endDate);
	list <Meeting> result = m_storage->queryMeeting([userName, start, 
		end] (Meeting it) -> bool {
		return ((userName == it.getSponsor() || it.isParticipator(userName)) && 
			(it.getStartDate() >= start || it.getEndDate() <= end));
	});
	return result;
}

list<Meeting> AgendaService::listAllMeetings(const string userName) const {
	list<Meeting> result = m_storage->queryMeeting([userName] 
		(Meeting it) -> bool {
			if (userName == it.getSponsor() || it.isParticipator(userName))
				return true;
			else
				return false;
		});
	return result;
}

list<Meeting> AgendaService::listAllSponsorMeetings(const string userName) const {
	list<Meeting> result = m_storage->queryMeeting([userName] (Meeting it) -> bool {
		if (userName == it.getSponsor())
			return true;
		else
			return false;
	});
	return result;
}

list<Meeting> AgendaService::listAllParticipateMeetings(const string userName) const {
	list<Meeting> result = m_storage->queryMeeting([userName] (Meeting it) -> bool {
		if(it.isParticipator(userName))
			return true;
		else
			return false;
	});
	return result;
}

bool AgendaService::deleteMeeting(const string userName, const string title) {
	int count = m_storage->deleteMeeting([userName, title] 
		(Meeting it) -> bool {
			if (title == it.getTitle() && userName == it.getSponsor())
				return true;
			return false;
		});
	if (count != 0)
		return true;
	else
		return false;
}

bool AgendaService::deleteAllMeetings(const string userName) {
	int count = m_storage->deleteMeeting([userName] 
		(Meeting it) -> bool {
			if (userName == it.getSponsor())
				return true;
			return false;
		});
	if (count != 0)
		return true;
	else
		return false;
}

void AgendaService::startAgenda(void) {
	m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda(void) {
	m_storage->sync();
}