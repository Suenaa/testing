#include <iostream>
#include "Date.hpp"

using namespace std;

Date::Date() {
	m_year = 0;
	m_month = 0;
	m_day = 0;
	m_hour = 0;
	m_minute = 0;
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, 
	int t_minute) {
	m_year = t_year;
	m_month = t_month;
	m_day = t_day;
	m_hour = t_hour;
	m_minute = t_minute;
}

Date::Date(string dateString) {
	/*bool check;
	Date temp;
	if (dateString[4] != '-' || dateString[7] != '-' 
		|| dateString[10] != '/' || dateString[13] != ':') {
		check = false;
	}
	else {
		int timeNum[12], count = 0;
		for (int i = 0; i < 16; i++) {
			if (i != 4 && i != 7 && i != 10 && i != 13) {
				if (dateString[i] < 48 || dateString[i] > 57) {
					check = false;
				}
				timeNum[count] = (int)dateString[i] - 48;
				count++;
			}
		}
		temp.m_year = timeNum[0] * 1000 + timeNum[1] * 100 + timeNum[2] * 10 + timeNum[3];
		temp.m_month = timeNum[4] * 10 + timeNum[5];
		temp.m_day = timeNum[6] * 10 + timeNum[7];
		temp.m_hour = timeNum[8] * 10 + timeNum[9];
		temp.m_minute = timeNum[10] * 10 + timeNum[11];
	}
	if (check == false) {
		m_year = 0;
		m_month = 0;
		m_day = 0;
		m_hour = 0;
		m_minute = 0;
	}
	else {
		m_year = temp.m_year;
		m_month = temp.m_month;
		m_day = temp.m_day;
		m_hour = temp.m_hour;
		m_minute = temp.m_minute;
	}*/
	bool check = true;
	Date temp;
	if (dateString[4] != '-' || dateString[7] != '-' 
		|| dateString[10] != '/' || dateString[13] != ':') {
		check = false;
	}
	else {
		int timeNum[12], count = 0;
		for (int i = 0; i < 16; i++) {
			if (i != 4 && i != 7 && i != 10 && i != 13) {
				if (dateString[i] < 48 || dateString[i] > 57) {
					check = false;
				}
				timeNum[count] = (int)dateString[i] - 48;
				count++;
			}
		}
		temp.m_year = timeNum[0] * 1000 + timeNum[1] * 100 + timeNum[2] * 10 + timeNum[3];
		temp.m_month = timeNum[4] * 10 + timeNum[5];
		temp.m_day = timeNum[6] * 10 + timeNum[7];
		temp.m_hour = timeNum[8] * 10 + timeNum[9];
		temp.m_minute = timeNum[10] * 10 + timeNum[11];
	}
	if (check == false) {
		m_year = 0;
		m_month = 0;
		m_day = 0;
		m_hour = 0;
		m_minute = 0;
	}
	else {
		m_year = temp.m_year;
		m_month = temp.m_month;
		m_day = temp.m_day;
		m_hour = temp.m_hour;
		m_minute = temp.m_minute;
	}
}

int Date::getYear(void) const {
	return m_year;
}

void Date::setYear(const int t_year) {
	m_year = t_year;
}

int Date::getMonth(void) const {
	return m_month;
}

void Date::setMonth(const int t_month) {
	m_month = t_month;
}

int Date::getDay(void) const {
	return m_day;
}

void Date::setDay(const int t_day) {
	m_day = t_day;
}

int Date::getHour(void) const {
	return m_hour;
}

void Date::setHour(const int t_hour) {
	m_hour = t_hour;
}

int Date::getMinute(void) const {
	return m_minute;
}

void Date::setMinute(const int t_minute) {
	m_minute = t_minute;
}

bool Date::isValid(const Date t_date) {
	if (t_date.m_year < 1000 || t_date.m_year > 9999 
		|| t_date.m_month > 12 || t_date.m_month < 1 
		|| t_date.m_hour < 0 || t_date.m_hour > 23 
		|| t_date.m_minute < 0 || t_date.m_minute > 59) {
		return false;
	}
	else {
		if (t_date.m_month == 2) {
			if ((t_date.m_year % 400 == 0) || (t_date.m_year % 100 != 0 
				&& t_date.m_year % 4 == 0)) {
				if (t_date.m_day > 29 || t_date.m_day < 1)
					return false;
				else
					return true;
			}
			else {
				if (t_date.m_day > 28 || t_date.m_day < 1)
					return false;
				else
					return true;
			}
		}
		else if (t_date.m_month == 4 || t_date.m_month == 6
			|| t_date.m_month == 9 || t_date.m_month == 11) {
			if (t_date.m_day > 30 || t_date.m_day < 1)
				return false;
			else
				return true;
		}
		else {
			if (t_date.m_day > 31 || t_date.m_day < 1)
				return false;
			else
				return true;
		}
	}
}

Date Date::stringToDate(const string t_dateString) {
    bool check = true;
	Date temp;
	if (t_dateString[4] != '-' || t_dateString[7] != '-' 
		|| t_dateString[10] != '/' || t_dateString[13] != ':') {
		check = false;
	}
	else {
		int timeNum[12], count = 0;
		for (int i = 0; i < 16; i++) {
			if (i != 4 && i != 7 && i != 10 && i != 13) {
				if (t_dateString[i] < 48 || t_dateString[i] > 57) {
					check = false;
				}
				timeNum[count] = (int)t_dateString[i] - 48;
				count++;
			}
		}
		temp.m_year = timeNum[0] * 1000 + timeNum[1] * 100 + timeNum[2] * 10 + timeNum[3];
		temp.m_month = timeNum[4] * 10 + timeNum[5];
		temp.m_day = timeNum[6] * 10 + timeNum[7];
		temp.m_hour = timeNum[8] * 10 + timeNum[9];
		temp.m_minute = timeNum[10] * 10 + timeNum[11];
	}
	if (check == false) {
		temp.m_year = 0;
		temp.m_month = 0;
		temp.m_day = 0;
		temp.m_hour = 0;
		temp.m_minute = 0;
	}
	return temp;
}

string Date::dateToString(Date t_date) {
    string temp;
    if (isValid(t_date)) {
    	temp = to_string(t_date.m_year) + "-";
    	if (t_date.m_month < 10)
    		temp = temp + "0" + to_string(t_date.m_month) + "-";
    	else 
    		temp = temp + to_string(t_date.m_month) + "-";
    	if (t_date.m_day < 10)
    		temp = temp + "0" + to_string(t_date.m_day) + "/";
    	else
    		temp = temp + to_string(t_date.m_day) + "/";
    	if (t_date.m_hour < 10)
    		temp = temp + "0" + to_string(t_date.m_hour) + ":";
    	else
    		temp = temp + to_string(t_date.m_hour) + ":";
    	if (t_date.m_minute < 10)
    		temp = temp + "0" + to_string(t_date.m_minute);
    	else
    		temp = temp + to_string(t_date.m_minute);
    }
    else {
    	temp = "0000-00-00/00:00";
    }
    return temp;
}

Date & Date::operator=(const Date &t_date) {
/*	Date tt_date;
	tt_date.m_year = t_date.m_year;
	tt_date.m_month = t_date.m_month;
	tt_date.m_day = t_date.m_day;
	tt_date.m_hour = t_date.m_hour;
	tt_date.m_minute = t_date.m_minute;
	return tt_date;
	*/
	m_year = t_date.m_year;
	m_month = t_date.m_month;
	m_day = t_date.m_day;
	m_hour = t_date.m_hour;
	m_minute = t_date.m_minute;
	return *this;
}

bool Date::operator==(const Date &t_date) const {
	if(m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day 
		&& m_hour == t_date.m_hour && m_minute == t_date.m_minute) 
		return true;
	else
		return false;
}

bool Date::operator>(const Date &t_date) const {
	if (m_year > t_date.m_year) {
		return true;
	}
	else if (m_year < t_date.m_year) {
		return false;
	}
	else {
		if (m_month > t_date.m_month) {
			return true;
		}
		else if (m_month < t_date.m_month) {
			return false;
		}
		else {
			if (m_day > t_date.m_day) {
				return true;
			}
			else if (m_day < t_date.m_day) {
				return false;
			}
			else {
				if (m_hour > t_date.m_hour) {
					return true;
				}
				else if (m_hour < t_date.m_hour) {
					return false;
				}
				else {
					if (m_minute > t_date.m_minute)
						return true;
					else
						return false;
				}
			}
		}
	}
}

bool Date::operator<(const Date &t_date) const {
	if (m_year < t_date.m_year) {
		return true;
	}
	else if (m_year > t_date.m_year) {
		return false;
	}
	else {
		if (m_month < t_date.m_month) {
			return true;
		}
		else if (m_month > t_date.m_month) {
			return false;
		}
		else {
			if (m_day < t_date.m_day) {
				return true;
			}
			else if (m_day > t_date.m_day) {
				return false;
			}
			else {
				if (m_hour < t_date.m_hour) {
					return true;
				}
				else if (m_hour > t_date.m_hour) {
					return false;
				}
				else {
					if (m_minute < t_date.m_minute)
						return true;
					else
						return false;
				}
			}
		}
	}
}

bool Date::operator>=(const Date &t_date) const {
	if (m_year < t_date.m_year) {
		return false;
	} 
	else if (m_year > t_date.m_year) {
		return true;
	}
	else {
		if (m_month < t_date.m_month) {
			return false;
		}
		else if (m_month > t_date.m_month) {
			return true;
		}
		else {
			if (m_day < t_date.m_month) {
				return false;
			}
			else if (m_day > t_date.m_month) {
				return true;
			}
			else {
				if (m_hour < t_date.m_hour) {
					return false;
				}
				else if (m_hour > t_date.m_hour) {
					return true;
				}
				else {
					if (m_minute < t_date.m_minute)
						return false;
					else
						return true;
				}
			}
		}
	}
}

bool Date::operator<=(const Date &t_date) const {
	if (m_year > t_date.m_year) {
		return false;
	} 
	else if (m_year < t_date.m_year) {
		return true;
	}
	else {
		if (m_month > t_date.m_month) {
			return false;
		}
		else if (m_month < t_date.m_month) {
			return true;
		}
		else {
			if (m_day > t_date.m_month) {
				return false;
			}
			else if (m_day < t_date.m_month) {
				return true;
			}
			else {
				if (m_hour > t_date.m_hour) {
					return false;
				}
				else if (m_hour < t_date.m_hour) {
					return true;
				}
				else {
					if (m_minute > t_date.m_minute)
						return false;
					else
						return true;
				}
			}
		}
	}
}

