#include <iostream>
#include "AgendaService.hpp"
#include "Storage.hpp"
#include <initializer_list>

using namespace std;

AgendaService::AgendaService() {
	startAgenda();
}

AgendaService::~AgendaService() {
	quitAgenda();
}

/*bool AgendaService::userLogIn(const string userName, const string password) {
	auto fun = [userName, password] (User it) {
		return (userName == it.getName() && password == it.getPassword());
	};
	if ((m_storage->queryUser(fun)).size() != 0)
		return true;
	else 
		return false;
}*/
bool AgendaService::userLogIn(const string userName, const string password) {
	list<User> result;
	result = m_storage->queryUser([&] (const User & it) {
		return userName == it.getName() && password == it.getPassword();
	});
	return result.size() > 0;
}

bool AgendaService::userRegister(const string userName, const string password, 
	const string email, const string phone) {
	list<User> t_Users;
	t_Users = m_storage->queryUser([&] (const User &it) {
		return userName == it.getName();
	});
	if (t_Users.size() == 0) {
		User temp(userName, password, email, phone);
		m_storage->createUser(temp);
		return true;
	}
	else {
		return false;
	}
	/*if (m_storage->queryUser([&](const User &it){
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
	}*/
}

bool AgendaService::deleteUser(const string userName, const string password) {
	int count = 0;
	count = m_storage->deleteUser([&] (const User& it) {
		if (userName == it.getName() && password == it.getPassword())
			return true;
		return false;
	});	
	if (count == 0) {
		return false;
	}
	else {
		deleteAllMeetings(userName);
		return true;
	}
}

list<User> AgendaService::listAllUsers(void) const {
	list<User> result;
	result = m_storage->queryUser([] (const User& it) {
		return true;
	});
	return result;
}

bool AgendaService::createMeeting(const string userName, const string title,
	const string startDate, const string endDate, const vector<string> participator) {
	if (!(Date::isValid(Date::stringToDate(startDate)) &&
		Date::isValid(Date::stringToDate(endDate))))
		return false;
	Date start(startDate);
	Date end(endDate);
	if (start >= end)
		return false;
	Meeting temp(userName, participator, start, end, title);
	if (participator.size() <= 0)
		return false;
	auto filter = [userName](const User& it) {
		return userName == it.getName();
	};
	if ((m_storage->queryUser(filter)).size() <= 0)
		return false;
	for (auto i = participator.begin(); i != participator.end(); i++) {
		list<User> t_user = m_storage->queryUser([&] (const User& it) {
				return *i == it.getName();
			});
		if (t_user.size() == 0)
			return false;
		if (t_user.size() < participator.size())
			return false;
	}
	for (auto i = participator.begin(); i != participator.end(); i++) {
		if (userName == *i)
			return false;
	}
	list <Meeting> result;
	result = m_storage->queryMeeting([&] (const Meeting& it) {
		if (userName == it.getSponsor() || it.isParticipator(userName)) {
			if(end > it.getStartDate() && it.getEndDate() > start)
				return true;
			else
				return false;
		}
		else
			return false;
	});
	if (result.size() > 0)
		return false;
	for (auto i = participator.begin(); i != participator.end(); i++) {
		list<Meeting> result2 = m_storage->queryMeeting([&] 
			(const Meeting & it) {
				if (*i == it.getSponsor() || it.isParticipator(*i)) { 
					if (end > it.getStartDate() && start < it.getEndDate()) 
						return true;
				}
				else
					return false;
			});
		if (result2.size() > 0)
			return false;
	}
	auto ifTitle = [&](const Meeting& it) {
		return title == it.getTitle();
	};
	if (m_storage->queryMeeting(ifTitle).size() > 0)
		return false;
	m_storage->createMeeting(temp);
	return true;
}

list<Meeting> AgendaService::meetingQuery(const string userName, string title) const {
	list<Meeting> result;
	result = m_storage->queryMeeting([&] (const Meeting& it) {
		if (title == it.getTitle() && (userName == it.getSponsor() || 
			it.isParticipator(userName)))
			return true;
		else
			return false;
	});
	return result;
}

list<Meeting> AgendaService::meetingQuery(const string userName, const string startDate, const string endDate) const {
	Date start = Date::stringToDate(startDate), end = Date::stringToDate(endDate);
	auto filter = [&] (const Meeting& it) {
		if ((userName == it.getSponsor() || it.isParticipator(userName)) && !(end < it.getStartDate() || start > it.getEndDate()))
			return true;
		else
			return false;
	};
	return m_storage->queryMeeting(filter);
}

list<Meeting> AgendaService::listAllMeetings(const string userName) const {
	return m_storage->queryMeeting([&] (const Meeting& it) {
			if (userName == it.getSponsor() || it.isParticipator(userName))
				return true;
			else
				return false;
		});
}

list<Meeting> AgendaService::listAllSponsorMeetings(const string userName) const {
	return m_storage->queryMeeting([&] (const Meeting& it) {
		if (userName == it.getSponsor())
			return true;
		else
			return false;
	});
}

list<Meeting> AgendaService::listAllParticipateMeetings(const string userName) const {
	return m_storage->queryMeeting([&] (const Meeting& it) {
		if(it.isParticipator(userName))
			return true;
		else
			return false;
	});
}

bool AgendaService::deleteMeeting(const string userName, const string title) {
	int count = 0;
	count = m_storage->deleteMeeting([&] (const Meeting& it) {
			if (title == it.getTitle() && userName == it.getSponsor())
				return true;
			return false;
		});
	if (count)
		return true;
	else
		return false;
}

bool AgendaService::deleteAllMeetings(const string userName) {
	int count = 0;
	count = m_storage->deleteMeeting([&] (const Meeting& it) {
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
}