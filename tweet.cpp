#include <iostream>
#include <string>
#include <string>
#include <vector>
#include <set>
#include "tweet.h"


using namespace std;

Tweet::Tweet()
: time_()
{
}

Tweet::Tweet(User* user, const DateTime& time, const std::string& text)
: user_(user), time_(time), text_(text)
{
}

DateTime const & Tweet::time() const
{
	return time_;
}

std::string const & Tweet::text() const
{
	return text_;
}

std::set<std::string> Tweet::hashTags() const
{
	return hashtag_;
}

bool Tweet::operator<(const Tweet& other) const
{
	return this->time_ < other.time_;
}

std::ostream& operator<<(std::ostream& os, const Tweet& t)
{
	os << t.time_ << " " << t.user_->name() << " " << t.text_;
	return os;
}

User* Tweet::user() const
{
	return user_;
}