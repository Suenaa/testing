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
	Date start;
	Date end;
	start = Date::stringToDate(startDate);
	end = Date::stringToDate(endDate);
	if(!Date::isValid(start)) {
		return false;
	}
	if(!Date::isValid(end)) {
		return false;
	}
	if (start >= end) {
		return false;
	}
	if (participator.empty())
		return false;
	list<User> t_user;
	list<Meeting> t_meeting;
	// check participators
	for (auto it = participator.begin(); it != participator.end(); it++) {
		if (*it == userName) {
			return false;
		}
		string participant = *it;
		t_user = m_storage->queryUser([&participant] (const User& a) {
			if (a.getName() == participant) {
				return true;
			}
			else {
				return false;
			}
		});
		if (t_user.empty()) {
			return false;
		}
		t_meeting = m_storage->queryMeeting([&participant, &start, &end] (const Meeting& a) {
			if (a.getSponsor() == participant || a.isParticipator(participant)) {
				if (a.getStartDate() <= start && a.getEndDate() > start) {
					return true;
				}
				if (a.getStartDate() < end && a.getEndDate() >= end) {
					return true;
				}
				if (a.getStartDate() >= start && a.getEndDate() <= end) {
					return true;
				}
			}
			else {
				return false;
			}
		});
		if (!t_meeting.empty()) {
			return false;
		}
	}
	// check sponsor
	t_user = m_storage->queryUser([&](const User& a) {
		if (a.getName() == userName) {
			return true;
		}
		else {
			return false;
		}
	});
	if (t_user.empty()) {
		return false;
	}
	t_meeting = m_storage->queryMeeting([&userName, &start, &end] (const Meeting& a) {
		if (a.getSponsor() == userName || a.isParticipator(userName)) {
			if (a.getStartDate() <= start && a.getEndDate() > start) {
				return true;
			}
			if (a.getStartDate() < end && a.getEndDate() >= end) {
				return true;
			}
			if (a.getStartDate() >= start && a.getEndDate() <= end) {
				return true;
			}
		}
		else {
			return false;
		}
	});
	if (!t_meeting.empty()) {
		return false;
	}
	// check title
	t_meeting = m_storage->queryMeeting([&](const Meeting& a) {
		if (a.getTitle() == title) {
			return true;
		}
		else {
			return false;
		}
	});
	if (!t_meeting.empty()) {
		return false;
	}
	Meeting temp(userName, participator, start, end, title);
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
	if (!Date::isValid(start)) {
		list<Meeting> t_meeting;
		return t_meeting;
	}
	if (!Date::isValid(end)) {
		list<Meeting> t_meeting;
		return t_meeting;
	}
	auto filter = [&] (const Meeting& it) {
		if (userName == it.getSponsor() || it.isParticipator(userName)) {
			if (it.getStartDate() <= start && it.getEndDate() >= start) {
				return true;
			}
			if (it.getStartDate() <= end && it.getEndDate() >= end) {
				return true;
			}
			if (it.getStartDate() >= start && it.getEndDate() <= end) {
				return true;
			}
		}
		else {
			return false;
		}
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
