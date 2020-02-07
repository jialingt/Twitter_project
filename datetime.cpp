#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <iomanip>
#include <sstream>
#include "datetime.h"

using namespace std;

DateTime::DateTime()
{
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	year = timeinfo->tm_year+1900;
	month = timeinfo->tm_mon + 1;
	day = timeinfo->tm_mday;
	hour = timeinfo->tm_hour;
	min = timeinfo->tm_min;
	sec = timeinfo->tm_sec;
}

// Constructor with datetime information
DateTime::DateTime(int hh,int mm, int ss,int year, int month, int day)
: hour(hh), min(mm), sec(ss), year(year), month(month), day(day)
{
}

// Compare the datetime from year to second
bool DateTime::operator<(const DateTime& other) const
{
	if(this->year < other.year)
	{
		return true;
	}
	else if(this->year > other.year)
	{
		return false;
	}
	else
	{
		if(this->month < other.month)
		{
			return true;
		}
		else if(this->month > other.month)
		{
			return false;
		}
		else
		{
			if(this->day < other.day)
			{
				return true;
			}
			else if(this->day > other.day)
			{
				return false;
			}
			else
			{
				if(this->hour < other.hour)
				{
					return true;
				}
				else if(this->hour > other.hour)
				{
					return false;
				}
				else
				{
					if(this->min < other.min)
					{
						return true;
					}
					else if(this->min > other.min)
					{
						return false;
					}
					else
					{
						if(this->sec < other.sec)
						{
							return true;
						}
						else
						{
							return false;
						}
					}
				}
			}
		}
	}
}

// cout the datetime 
std::ostream& operator<<(std::ostream& os, const DateTime& other)
{
	os << other.year << "-" << setw(2) << setfill('0') << other.month << "-" << 
	setw(2) << setfill('0')<< other.day << " "  << setw(2) << setfill('0') <<
	 other.hour << ":" << setw(2) << setfill('0') << other.min << ":" << setw(2) << 
	 setfill('0') << other.sec;
	return os;
}

// cin the datetime
std::istream& operator>>(std::istream& is, DateTime& dt)
{
	string ymd;
	string hms;
	bool b =  false;
	// read the year month and day
	is >> ymd;
	// read the hour minute and second
	is >> hms;
	stringstream part1(ymd);
	stringstream part2(hms);
	string num;
	getline(part1,num,'-');
	if(num.empty() || part1.fail())
	{
		b = true;
	}
	else
	{
		stringstream ss(num);
		ss >> dt.year;
		if(ss.fail())
		{
			b = true;
		}
	}
	getline(part1,num,'-');
	if(num.empty() || part1.fail())
	{
		b = true;
	}
	else
	{
		stringstream sm(num);
		sm >> dt.month;
		if(sm.fail())
		{
			b = true;
		}
	}
	part1 >> dt.day;
	if(is.fail())
	{
		b = true;
	}
	getline(part2,num,':');
	if(num.empty() || part1.fail())
	{
		b = true;
	}
	else
	{
		stringstream sh(num);
		sh>>dt.hour;
		if(sh.fail())
		{
			b = true;
		}
	}
	getline(part2,num,':');
	if(num.empty() || part1.fail())
	{
		b = true;
	}
	else
	{
		stringstream smin(num);
		smin >> dt.min;
		if(smin.fail())
		{
			b = true;
		}
	}
	part2 >> dt.sec;
	if(is.fail())
	{
		b = true;
	}
	if(b == true)
	{
		time_t rawtime;
		struct tm* timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		dt.year = timeinfo->tm_year+1900;
		dt.month = timeinfo->tm_mon + 1;
		dt.day = timeinfo->tm_mday;
		dt.hour = timeinfo->tm_hour;
		dt.min = timeinfo->tm_min;
		dt.sec = timeinfo->tm_sec;
	}
	return is;
}