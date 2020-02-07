#include <map>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>
#include <ctime>
#include <stack>
#include <algorithm>
#include "twiteng.h"
#include "util.h"
#include "datetime.h"
#include "hsort.h"
#include "heap.h"
#include "tagdata.h"
#include "hash.h"

using namespace std;

TwitEng::TwitEng() : trending_(2), log_status(false)
{

}

// Twiteng destructor
TwitEng::~TwitEng()
{
	for(map< string, User*>::iterator it = all_users.begin();
		it != all_users.end(); ++it)
	{
		delete (it->second);
	}
}

// return if it can read the file properly
bool TwitEng::parse(char* filename)
{
	ifstream ifile(filename);
	if(!ifile.fail())
	{
		string first_line;
		getline(ifile, first_line);
		stringstream f_ss(first_line);
		f_ss >> num;
		if(!ifile.fail())
		{
			// Store the information about the follower and following relation 
			// and the password
			for(int i = 0; i < num; i++)
			{
				string user_line;
				getline(ifile, user_line);
				if(!user_line.empty())
				{
					stringstream user_info(user_line);
					string user_name;
					user_info >> user_name;
					if(user_info.fail())
					{
						return true;
					}
					User* new_user;
					if(all_users.find(user_name) == all_users.end())
					{
						new_user = new User(user_name);
						all_users.insert(make_pair(user_name, new_user));
					}
					else
					{
						new_user = all_users.find(user_name)->second;
					}
					unsigned int password;
					user_info >> password;
					pw_map.insert(make_pair(user_name, password));
					string following_name;
					User* user_folg;
					while(user_info >> following_name)
					{
						if(user_info.fail())
						{
							return true;
						}
						// Put the info into the all users information map
						if(all_users.find(following_name) == all_users.end())
						{
							user_folg = new User(following_name);
							all_users.insert(make_pair(following_name, user_folg));
						}
						else
						{
							user_folg = all_users.find(following_name)->second;
						}
						new_user->addFollowing(user_folg);
						user_folg->addFollower(new_user);
					}
				}
			}
			// Collect the tweet info
			string tweet_line;
			while(getline(ifile,tweet_line))
			{
				if(!tweet_line.empty())
				{
					stringstream tweet_info(tweet_line);
					DateTime time;
					string username;
					string fulltext;
					string parttext;
					string text;
					tweet_info >> time;
					if(tweet_info.fail())
					{
						return true;
					} 
					tweet_info >> username;
					if(tweet_info.fail() || username.find(":") !=std::string::npos || 
						username.find("-")!=std::string::npos)
					{
						return true;
					} 
					getline(tweet_info,fulltext);
					stringstream ss(fulltext);
					string first_word;
					ss >> first_word;
					if(ss.fail())
					{
						return true;
					}
					text = first_word;
					getline(ss,parttext);
					stringstream spart(parttext);
					string text_word;
					while(spart >> text_word)
					{
						text = text + " " + text_word;
					}
					addTweet(username, time, text);
				}
			}
			// Put the original tweet info into the heap

			/*for(map< string, set<Tweet*> >::iterator it = hashtwi.begin();
				it != hashtwi.end(); ++it)
			{
				string cur_hash = it->first;
				set<Tweet*> cor_tweet = it->second;
				int i = int(cor_tweet.size());
				TagData old(cur_hash, 0);
				TagData new_hastw(cur_hash, i);
				trending_.decreaseKey(old, new_hastw);
			}*/
			/*for(map<string, unsigned int>::iterator it = pw_map.begin();
				it != pw_map.end(); ++it)
			{
				cout << it->first << it->second << endl;
			}*/
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		// return that the file cannot be opened
		return true;
	}

	return false;
}

void TwitEng::addTweet(const std::string& username, const DateTime& time, 
	const std::string& text)
{
	Tweet* t = new Tweet(all_users.find(username)->second, time, text);
	all_users.find(username)->second->addTweet(t);
	stringstream hashtager(text);
	string temp_b; // a string to store the hashtag
	string cur_h;
	string old_h;
	while(getline(hashtager, temp_b,'#'))
	{
		hashtager >> cur_h;
		//Change the current hashtag to upper level
		convUpper(cur_h);
		if(!cur_h.empty())
		{
			// if it is a new hashtag
			if(hashtwi.find(cur_h) == hashtwi.end())
			{
				set<Tweet*> newtwiset;
				newtwiset.insert(t);
				hashtwi.insert(make_pair(cur_h, newtwiset));
				TagData new_hastw(cur_h, 1);
				trending_.push(new_hastw);
			}
			else
			{
				if(old_h == cur_h)
				{
					return;
				}
				int num = int((hashtwi.find(cur_h)->second).size());
				hashtwi.find(cur_h)->second.insert(t);
				int new_num = num + 1;
				TagData update(cur_h, new_num);
				TagData old(cur_h, num);
				trending_.decreaseKey(old, update);
			}
			old_h = cur_h;
		}
	}
	hashtager >> cur_h;
}

void TwitEng::dumpFeeds()
{
	mentioninfo();
	for(map< string, User*>::iterator it = all_users.begin();
		it != all_users.end(); ++it)
	{
		//cout << "here to mentions" << endl;
		string name_t_f = it->first;
		string file_ext(".feed");
		string file_name = name_t_f + file_ext;
		char file[file_name.size() + 1];
		strcpy(file, file_name.c_str());
		ofstream ofile(file);
		ofile << name_t_f << endl;
		User* info_t_f = it->second;
		vector<Tweet*> tweet_list = info_t_f->getFeed();
		for(size_t i = 0; i < tweet_list.size();i++)
		{
			ofile << *(tweet_list[i]) << endl;
		}
		// if there is no mention tweet of the user set the tweet map as empty
		if(mentionmap.find(name_t_f) == mentionmap.end())
		{
			vector<Tweet*> empty;
			mentionmap.insert(make_pair(name_t_f, empty));
		}
	}
	for(map< string, vector<Tweet*> >::iterator it = mentionmap.begin();
		it != mentionmap.end(); ++it)
	{
		string name_t_f = it->first;
		string file_ext(".mentions");
		string file_name = name_t_f + file_ext;
		char file[file_name.size() + 1];
		strcpy(file, file_name.c_str());
		ofstream ofile(file);
		ofile << name_t_f << endl;
		vector<Tweet*> tweet_list = it->second;
		hsort(tweet_list, TweetComp());
		//sort(tweet_list.begin(),tweet_list.end(),TweetComp());
		for(vector<Tweet*>::iterator tt = tweet_list.begin();
			tt != tweet_list.end(); ++tt)
		{
			ofile << *(*tt) << endl;
		}
	}
}

std::vector<Tweet*> TwitEng::search(std::vector<std::string>& terms, int strategy)
{
	//Call the AND search
	vector<Tweet*> v;
	if(strategy == 0)
	{
		return ANDsearch(terms);
	}
	else if(strategy == 1)
	{
		return ORsearch(terms);
	}
	return v;
}

std::set<Tweet*> TwitEng::intersect(const std::set<Tweet*>& s1, 
                                const std::set<Tweet*>& s2)
{
	// The code below can works for empty list, so there is no need
	// to write another case
	std::set<Tweet*> result;
	for(std::set<Tweet*>::iterator it = s1.begin();
		it != s1.end(); ++it)
	{
		Tweet* curritem = *it;
		// The running time is log(n)
		if(s2.find(curritem) != s2.end())
		{
			result.insert(curritem);
		}
	}
	// Since the number of tweets mathcing any single hashtag 
	// can be considered as constant. The total running time is log(n)
	return result;
}

std::vector<Tweet*> TwitEng::ANDsearch(std::vector<std::string>& terms)
{
	// Convert the string to upper level character
	set< set<Tweet*> > twitsum;
	vector<Tweet*> andresult;
	vector<Tweet*> emptyresult;
	set<Tweet*> resultset;
	// Find out the terms and put the corresponing set result into a set
	// The running time is klogn
	for(size_t i = 0; i < terms.size(); i++)
	{
		convUpper(terms[i]);
		map< string, set<Tweet*> >::iterator setptr = hashtwi.find(terms[i]);
		if(setptr != hashtwi.end())
		{
			twitsum.insert(setptr->second);
		}
		else
		{
			return emptyresult;
		}
	}
	resultset=*(twitsum.begin());
	//The running time is klogn
	for(set< set<Tweet*> >::iterator it = twitsum.begin();
		it != twitsum.end(); ++it)
	{
		// The running time for each intersect is logn since the number of
		// tweets matching any single hashtag can be considered as constant
		resultset = intersect(resultset,*it);
	}
	for(set<Tweet*>::iterator it = resultset.begin();
		it != resultset.end(); ++it)
	{
		andresult.push_back(*it);
	}
	return andresult;
}

std::set<Tweet*> TwitEng::uni(const std::set<Tweet*>& s1, 
                                const std::set<Tweet*>& s2)
{
	std::set<Tweet*> result;
	for(std::set<Tweet*>::iterator it = s1.begin();
		it != s1.end(); ++it)
	{
		Tweet* curritem = *it;
		// The running time is log(n)
		if(result.find(curritem) == result.end())
		{
			result.insert(curritem);
		}
	}
	for(std::set<Tweet*>::iterator it = s2.begin();
		it != s2.end(); ++it)
	{
		Tweet* curritem = *it;
		if(result.find(curritem) == result.end())
		{
			result.insert(curritem);
		}
	}
	// The total running time is log(n)
	return result;
}
std::vector<Tweet*> TwitEng::ORsearch(std::vector<std::string>& terms)
{
	set< set<Tweet*> > twitsum;
	vector<Tweet*> andresult;
	vector<Tweet*> emptyresult;
	set<Tweet*> resultset;
	set<Tweet*> emptyset;
	// Convert the string to upper level character
	// Find out the terms and put the corresponing set result into a set
	// The running time is klogn
	for(size_t i = 0; i < terms.size(); i++)
	{
		convUpper(terms[i]);
		map< string, set<Tweet*> >::iterator setptr = hashtwi.find(terms[i]);
		if(setptr != hashtwi.end())
		{
			twitsum.insert(setptr->second);
		}
		else
		{
			twitsum.insert(emptyset);
		}
	}
	resultset=*(twitsum.begin());
	//The running time is klogn
	for(set< set<Tweet*> >::iterator it = twitsum.begin();
		it != twitsum.end(); ++it)
	{
		// The running time for each intersect is logn since the number of
		// tweets matching any single hashtag can be considered as constant
		resultset = uni(resultset,*it);
	}
	for(set<Tweet*>::iterator it = resultset.begin();
		it != resultset.end(); ++it)
	{
		andresult.push_back(*it);
	}
	return andresult;
}

// Add the new tweet
void TwitEng::newtweet(string name, string text)
{
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	DateTime time(timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, 
		timeinfo->tm_year+1900, timeinfo->tm_mon + 1, timeinfo->tm_mday);
	//Tweet* t = new Tweet(all_users.find(name)->second, time, text);
	addTweet(name, time, text);
}

// TO check if the user name exists or not 
bool TwitEng::exist(const string name)
{
	if(all_users.find(name) != all_users.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

// ADD new follower and following
void TwitEng::andfollowinfo(string follower, string name_to_follow)
{
	// if the name to follow in the users data
	User* folr;
	User* folg;
	if(all_users.find(name_to_follow) != all_users.end())
	{
		folg = all_users.find(name_to_follow)->second;
		if(all_users.find(follower) != all_users.end())
		{
			folr = all_users.find(follower)->second;
			folr->addFollowing(folg);
			folg->addFollower(folr);
		}
	}
}

void TwitEng::Savehelper(string filename)
{
	ofstream ofile(filename.c_str());
	ofile << num << endl;
	for(map< string, User*>::iterator it = all_users.begin();
		it != all_users.end(); ++it)
	{
		ofile << it->first << " ";
		unsigned int password = pw_map.find(it->first)->second;
		ofile << password << " ";
		set<User*> folg = it->second->following();
		for(set<User*>::iterator jt = folg.begin();
			jt != folg.end(); ++jt)
		{
			ofile << (*jt)->name() <<  " ";
		}
		ofile << endl;
	}
	for(map< string, User*>::iterator it = all_users.begin();
		it != all_users.end(); ++it)
	{
		string name = it->first;
		list<Tweet*> tweet_collect = it->second->tweets();
		for(list<Tweet*>::iterator jt = tweet_collect.begin();
			jt != tweet_collect.end(); ++jt)
		{
			ofile << (*jt)->time() << " " << name <<  " " << (*jt) ->text() << endl;
		}
	}
}

void TwitEng::mentioninfo()
{
	for(map<string, User*>::iterator it = all_users.begin();
		it != all_users.end(); ++it)
	{
		list<Tweet*> user_tweets = it->second->tweets();
		for(list<Tweet*>::iterator tt = user_tweets.begin();
			tt != user_tweets.end(); ++tt)
		{
			string text_info = (*tt)->text();
			// if the first letter is @
			if(text_info[0] == '@')
			{
				stringstream ss(text_info);
				string mentioned_name;
				ss >> mentioned_name;
				mentioned_name.erase(0,1);
				// if the name is not in the mentionmap
				if(mentionmap.find(mentioned_name) == mentionmap.end())
				{
					vector<Tweet*> mention_tweets;
					mention_tweets.push_back(*tt);
					mentionmap.insert(make_pair(mentioned_name, mention_tweets));
				}
				else
				{
					// if the name is already in the mentionmap*/
					map< string, vector<Tweet*> >::iterator exist_loc = 
								mentionmap.find(mentioned_name);
					exist_loc->second.push_back(*tt);
				}
			}
			// if the tweet is not begin with '@'
			else
			{
				stringstream ss(text_info);
				string single_word;
				while(ss >> single_word)
				{
					if(single_word[0] == '@')
					{
						string mentioned_name = single_word;
						mentioned_name.erase(0,1);
						if(mentionmap.find(mentioned_name) == mentionmap.end())
						{
							vector<Tweet*> mention_tweets;
							mention_tweets.push_back(*tt);
							mentionmap.insert(make_pair(mentioned_name, mention_tweets));
						}
						else
						{
							map< string, vector<Tweet*> >::iterator exist_loc = 
													mentionmap.find(mentioned_name);
							exist_loc->second.push_back(*tt);
						}
					}
				}
			}
		}
	}

}

void TwitEng::SCC(string filename)
{
	// index start from 1 to avoid the repetition with 0
	// otherwise there will be infinite loop
	int index = 1;
	int scc_index = 1;
	stack<User*> user_stack;
	set<User*> visited_user;
	for(map< string, User* >::iterator it = all_users.begin();
		it != all_users.end(); ++it)
	{
		// if the user index equals to zero, it is undefined
		if(it->second->index() == 0)
		{
			// the increment of index in done in the helper function
			SCC_helper(it->second, index, user_stack, visited_user, scc_index);

		}

	}
	SCC_print(filename);
}

void TwitEng::SCC_helper(User* user, int& index, stack<User*>& user_s, 
	set<User*>& visited, int& scc_index)
{
	user->index() = index;
	user->lowlink() = index;
	index++;
	user_s.push(user);
	visited.insert(user);
	set<User*> following = user->following();
	for(set<User*>::iterator it = following.begin();
		it != following.end(); ++it)
	{
		// if the index is undefined
		if((*it)->index() == 0)
		{
			SCC_helper(*it, index, user_s, visited, scc_index);
			user->lowlink() = min(user->lowlink(), (*it)->lowlink());
		}
		// if the user is on the stack
		else if(visited.find(*it) != visited.end())
		{
			user->lowlink() = min(user->lowlink(), (*it)->index());
		}
	}
	if(user->lowlink() == user->index())
	{
		vector<User*> scc_part;
		User* w;
		do
		{
			w = user_s.top();
			user_s.pop();
			visited.erase(visited.find(w));
			scc_part.push_back(w);
		}while(user->name() != w->name());
		scc_map.insert(make_pair(scc_index, scc_part));
		scc_index++;
	}
}

void TwitEng::SCC_print(string filename)
{
	// Print out the result
	ofstream ofile(filename.c_str());
	for(map<int, vector<User*> >::iterator it = scc_map.begin();
		it != scc_map.end(); ++it)
	{
		ofile << "Component " << it->first << endl;
		for(size_t i = 0; i < it->second.size(); i++)
		{
			ofile << (it->second[i])->name() << endl;
		}
		ofile << endl;
	}
	ofile.close();
}

void TwitEng::trend_print(int n)
{
	vector<TagData> store_;
	for(int i = 0; i < n; i++)
	{		
		cout << (trending_.top()).tag << " : " << (trending_.top()).num << endl;
		store_.push_back(trending_.top());
		trending_.pop();
	}
	for(size_t i = 0; i < store_.size(); i++)
	{
		trending_.push(store_[i]);
	}
}

size_t TwitEng::hash_num()
{
	return hashtwi.size();
}

bool TwitEng::login(string user_name, string password)
{
	unsigned int result = calculateHash(password);
	if(((pw_map.find(user_name))->second) == result)
	{
		curr_user = user_name;
		log_status = true;
		return true;
	}
	else
	{
		return false;
	}
}

bool TwitEng::log_sta()
{
	return log_status;
}

string TwitEng::log_user()
{
	return curr_user;
}

void TwitEng::logout()
{
	log_status = false;
}