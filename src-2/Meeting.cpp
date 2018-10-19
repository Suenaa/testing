#include <iostream>
#include <string>
#include "Meeting.hpp"

using namespace std;

Meeting::Meeting(string t_sponsor, vector<string> t_participator, 
	Date t_startTime, Date t_endTime, string t_title) {
	m_sponsor = t_sponsor;
	m_participators.assign(t_participator.begin(), t_participator.end());
	m_startDate = t_startTime;
	m_endDate = t_endTime;
	m_title = t_title;
}

Meeting::Meeting(const Meeting &t_meeting) {
	m_sponsor = t_meeting.m_sponsor;
	m_participators.assign(t_meeting.m_participators.begin(), t_meeting.m_participators.end());
	m_startDate = t_meeting.m_startDate;
	m_endDate = t_meeting.m_endDate;
	m_title = (t_meeting.m_title);
}

string Meeting::getSponsor(void) const {
	return m_sponsor;
}

void Meeting::setSponsor(const string t_sponsor) {
	m_sponsor = t_sponsor;
}

vector<string> Meeting::getParticipator(void) const {
	return m_participators;
}

void Meeting::setParticipator(const vector<string> t_participators) {
	m_participators.assign(t_participators.begin(), t_participators.end());
}

Date Meeting::getStartDate(void) const {
	return m_startDate;
}

void Meeting::setStartDate(const Date t_startTime) {
	m_startDate = t_startTime;
}

Date Meeting::getEndDate(void) const {
	return m_endDate;
}

void Meeting::setEndDate(const Date t_endTime) {
	m_endDate = t_endTime;
}

string Meeting::getTitle(void) const {
	return m_title;
}

void Meeting::setTitle(const string t_title) {
	m_title = t_title;
}

bool Meeting::isParticipator(const string t_username) const {
	for (int i = 0; i < m_participators.size(); i++) {
		if (m_participators[i] == t_username)
			return true;
	}
	return false;
}