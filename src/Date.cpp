#include <sstream>
#include "Date.hpp"

using namespace std;

Date::Date() {
	m_year = 0;
	m_month = 0;
	m_day = 0;
	m_hour = 0;
	m_minute = 0;
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
	m_year = t_year;
	m_month = t_month;
	m_day = t_day;
	m_hour = t_hour;
	m_minute = t_minute;
}


Date::Date(string dateString) {
	(*this) = stringToDate(dateString);
}

int Date::getYear() const {
	return m_year;
}

void Date::setYear(const int t_year) {
	m_year = t_year;
}

int Date::getMonth() const {
	return m_month;
}

void Date::setMonth(const int t_month) {
	m_month = t_month;
}

int Date::getDay() const {
	return m_day;
}

void Date::setDay(const int t_day) {
	m_day = t_day;
}

int Date::getHour() const {
	return m_hour;
}

void Date::setHour(const int t_hour) {
	m_hour = t_hour;
}

int Date::getMinute() const {
	return m_minute;
}

void Date::setMinute(const int t_minute) {
	m_minute = t_minute;
}

bool Date::isValid(const Date t_date) {
	int year = t_date.getYear();
    int month = t_date.getMonth();
    int day = t_date.getDay();
    int hour = t_date.getHour();
    int minute = t_date.getMinute();
    if (year < 1000 || year > 9999 || month < 1 ||
        month > 12 || day < 1 || t_date.getHour() < 0 ||
        t_date.getHour() >= 24 || t_date.getMinute() < 0 ||
        t_date.getMinute() >= 60) {
        return false;
    }

    if (month == 1 || month == 3 || month == 5 ||
        month == 7 || month == 8 || month == 10 ||
        month == 12) {
        if (day > 31) {
            return false;
        }
    } else if (month == 4 || month == 6 || month == 9 ||
               month == 11) {
        if (day > 30) {
            return false;
        }
    } else {
        //若年份为闰年，则2月29天
        if ((year % 4 == 0 && year % 100 != 0) ||
            (year % 400 == 0)) {
            if (day > 29) {
                return false;
            }
        } else {
            if (day > 28) {
                return false;
            }
        }
    }
    if(hour < 0 || hour > 24 || minute < 0 || minute > 60) {
    	return false;
    }
    return true;
}

int stringToInt(string s) {
	int result;
	stringstream st;
	st << s;
	st >> result;
	return result;
}

/**
* @brief convert a string to date, if the format is not correct return
* 0000-00-00/00:00
* @return a date
*/
Date Date::stringToDate(const string t_dateString) {
	Date result;
	if(t_dateString.length() != 16) {
		return result;
	}
	for (int i = 0; i < t_dateString.length(); i++) {
		switch(i) {
			case 4:
				if (t_dateString[i] != '-') {
					return result;
				}
				break;
			case 7:
				if (t_dateString[i] != '-') {
					return result;
				}
				break;
			case 10:
				if (t_dateString[i] != '/') {
					return result;
				}
				break;
			case 13:
				if (t_dateString[i] != ':') {
					return result;
				}
				break;
			default:
				if (t_dateString[i] > '9' || t_dateString[i] < '0') {
					return result;
				}
		}
	}

	result.setYear(stringToInt(t_dateString.substr(0, 4)));
	result.setMonth(stringToInt(t_dateString.substr(5, 2)));
	result.setDay(stringToInt(t_dateString.substr(8, 2)));
	result.setHour(stringToInt(t_dateString.substr(11, 2)));
	result.setMinute(stringToInt(t_dateString.substr(14, 2)));
	return result;
}

string intToString(int num) {
	string result;
	stringstream st;
	st << num;
	st >> result;
	if (result.length() == 1) {
		result = "0" + result;
	}
	return result;
}

  /**
  * @brief convert a date to string, if the format is not correct return
  * 0000-00-00/00:00
  */
string Date::dateToString(Date t_date) {
	string result = "";
	if(!isValid(t_date)) {
		return result;
	}
	result = intToString(t_date.getYear()) + "-" 
			 + intToString(t_date.getMonth()) + "-" 
			 + intToString(t_date.getDay()) + "/" 
			 + intToString(t_date.getHour()) + ":" 
			 + intToString(t_date.getMinute());
	return result;
}

  /**
  *  @brief overload the assign operator
  */
Date &Date::operator=(const Date &t_date) {
  	this->setYear(t_date.getYear());
    this->setMonth(t_date.getMonth());
    this->setDay(t_date.getDay());
    this->setHour(t_date.getHour());
    this->setMinute(t_date.getMinute());
    return (*this);
}

  /**
  * @brief check whether the CurrentDate is equal to the t_date
  */
bool Date::operator==(const Date &t_date) const {
	return (this->m_year == t_date.getYear() &&
		this->m_month == t_date.getMonth() && 
		this->m_day == t_date.getDay() && 
		this->m_hour == t_date.getHour() && 
		this->m_minute == t_date.getMinute());
}

  /**
  * @brief check whether the CurrentDate is  greater than the t_date
  */
bool Date::operator>(const Date &t_date) const {
	return dateToString(*this) > dateToString(t_date);
}

  /**
  * @brief check whether the CurrentDate is  less than the t_date
  */
bool Date::operator<(const Date &t_date) const {
	return dateToString(*this) < dateToString(t_date);
}

  /**
  * @brief check whether the CurrentDate is  greater or equal than the t_date
  */
bool Date::operator>=(const Date &t_date) const {
	return ((*this) == t_date || (*this) > t_date);
}

  /**
  * @brief check whether the CurrentDate is  less than or equal to the t_date
  */
bool Date::operator<=(const Date &t_date) const {
	return ((*this) == t_date || (*this) < t_date);
}