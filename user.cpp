#include <string>
#include <set>
#include <list>
#include <vector>
#include <sstream>
#include <algorithm>
#include "user.h"
#include "hsort.h"

using namespace std;

User::User(std::string name):
	name_(name)
{
	idx = 0;
	lowlk = 0;
}

User::~User()
{
	for(list<Tweet*>::iterator it = tweets_.begin();
			it != tweets_.end(); ++it)
	{
		delete *it;
	}
}

std::string User::name() const
{
	return name_;
}

std::set<User*> User::followers() const
{
	return follower_;
}

std::set<User*> User::following() const
{
	return following_;
}

std::list<Tweet*> User::tweets() const
{
	return tweets_;
}

void User::addFollower(User* u)
{
	follower_.insert(u);
}

void User::addFollowing(User* u)
{
	following_.insert(u);
}

void User::addTweet(Tweet* t)
{
	tweets_.insert(tweets_.end(), t);
}

int User::index() const
{
	return idx;
}

int User::lowlink() const
{
	return lowlk;
}

int& User::index()
{
	return idx;
}

int& User::lowlink()
{
	return lowlk;
}

std::vector<Tweet*> User::getFeed()
{
	vector<Tweet*> feedtweets;
	// push back the user's tweets
	for(list<Tweet*>::iterator it = tweets_.begin();
		it != tweets_.end(); ++it)
	{
		feedtweets.push_back(*it);
	}
	// push back the following users tweets
	for(set<User*>::iterator it = following_.begin();
		it != following_.end(); ++it)
	{
		
		if(follower_.find(*it) == follower_.end())
		{
			list<Tweet*> tweets_fol = (*it)->tweets_;
			for(list<Tweet*>::iterator follow_it = tweets_fol.begin();
				follow_it != tweets_fol.end(); ++follow_it)
			{
				string text_content = (*follow_it)->text();
				if(text_content[0] != '@')
				{
					feedtweets.push_back(*follow_it);
				}	
			}
		}
		else
		{
			list<Tweet*> tweets_fol = (*it)->tweets_;
			for(list<Tweet*>::iterator follow_it = tweets_fol.begin();
				follow_it != tweets_fol.end(); ++follow_it)
			{
				string text_content = (*follow_it)->text();
				if(text_content[0] != '@')
				{
					feedtweets.push_back(*follow_it);
				}
				else
				{
					stringstream ss(text_content);
					string men_name;
					ss >> men_name;
					men_name.erase(0,1);
					if(men_name == name_)
					{
						feedtweets.push_back(*follow_it);
					}
				}	
			}
		}
	}
	// Sort the tweets in timestamp order
	hsort(feedtweets, TweetComp());
	//sort(feedtweets.begin(),feedtweets.end(),TweetComp());
	return feedtweets;
}