#include <iostream>
#include <string>
#include "User.hpp"
#include "Date.hpp"
#include "Meeting.hpp"

using namespace std;

Meeting::Meeting(string t_sponsor, vector<string> t_participator, Date t_startTime,
          Date t_endTime, string t_title) {
	m_sponsor = t_sponsor;
	m_participators = t_participator;
	m_startDate = t_startTime;
	m_endDate = t_endTime;
	m_title = t_title;
}

Meeting::Meeting(const Meeting &t_meeting) {
	m_sponsor = t_meeting.getSponsor();
	m_participators = t_meeting.getParticipator();
	m_startDate = t_meeting.getStartDate();
	m_endDate = t_meeting.getEndDate();
	m_title = t_meeting.getTitle();
}

string Meeting::getSponsor() const {
	return m_sponsor;
}

  /**
  * @brief set the sponsor of a meeting
  * @param  the new sponsor string
  */
void Meeting::setSponsor(const string t_sponsor) {
	m_sponsor = t_sponsor;
}

  /**
  * @brief  get the participators of a meeting
  * @return return a string vector indicate participators
  */
vector<string> Meeting::getParticipator() const {
	return m_participators;
}

  /**
  *   @brief set the new participators of a meeting
  *   @param the new participators vector
  */
void Meeting::setParticipator(const vector<string> t_participators) {
	m_participators = t_participators;
}

  /**
  * @brief get the startDate of a meeting
  * @return return a string indicate startDate
  */
Date Meeting::getStartDate() const {
	return m_startDate;
}

  /**
  * @brief  set the startDate of a meeting
  * @param  the new startdate of a meeting
  */
void Meeting::setStartDate(const Date t_startTime) {
	m_startDate = t_startTime;
}

  /**
  * @brief get the endDate of a meeting
  * @return a date indicate the endDate
  */
Date Meeting::getEndDate() const {
	return m_endDate;
}

  /**
  * @brief  set the endDate of a meeting
  * @param  the new enddate of a meeting
  */
void Meeting::setEndDate(const Date t_endTime) {
	m_endDate = t_endTime;
}

  /**
  * @brief get the title of a meeting
  * @return a date title the endDate
  */
string Meeting::getTitle() const {
	return m_title;
}

  /**
  * @brief  set the title of a meeting
  * @param  the new title of a meeting
  */
void Meeting::setTitle(const string t_title) {
	m_title = t_title;
}


  /**
  * @brief check if the user take part in this meeting
  * @param t_username the source username
  * @return if the user take part in this meeting
  */
bool Meeting::isParticipator(const string t_username) const {
	for (auto it = m_participators.begin(); it != m_participators.end(); it++) {
		if(*it == t_username) {
			return true;
		}
	}
	return false;
}