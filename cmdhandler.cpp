#include "cmdhandler.h"
#include "util.h"
#include "twiteng.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cstring>
using namespace std;

QuitHandler::QuitHandler()
{

}

QuitHandler::QuitHandler(Handler* next)
  : Handler(next)
{

}

bool QuitHandler::canHandle(const std::string& cmd) const
{
	return cmd == "QUIT";

}

Handler::HANDLER_STATUS_T QuitHandler::process(TwitEng* eng, 
	std::istream& instr) const
{
	eng->dumpFeeds();
	return HANDLER_QUIT;
}

AndHandler::AndHandler()
{

}

AndHandler::AndHandler(Handler* next)
: Handler(next)
{

}

bool AndHandler::canHandle(const std::string& cmd) const
{
	return cmd == "AND";

}

// Collect the input term and call the and search
Handler::HANDLER_STATUS_T AndHandler::process(TwitEng* eng, 
	std::istream& instr) const
{
	vector<Tweet*> result;
	vector<std::string> user_term;
	string term;
	while(instr >> term)
	{
		user_term.push_back(term);
	}
	result=eng->search(user_term,0);
	displayHits(result);
	return HANDLER_OK;
}

OrHandler::OrHandler()
{

}

OrHandler::OrHandler(Handler* next)
: Handler(next)
{

}

bool OrHandler::canHandle(const std::string& cmd) const
{
	return cmd == "OR";

}

// Collect the input term and call the orsearch
Handler::HANDLER_STATUS_T OrHandler::process(TwitEng* eng, 
	std::istream& instr) const
{
	vector<Tweet*> result;
	vector<std::string> user_term;
	string term;
	while(instr >> term)
	{
		user_term.push_back(term);
	}
	result = eng->search(user_term,1);
	displayHits(result);
	return HANDLER_OK;
}

TweetHandler::TweetHandler()
{

}

TweetHandler::TweetHandler(Handler* next)
: Handler(next)
{

}

bool TweetHandler::canHandle(const std::string& cmd) const
{
	return cmd == "TWEET";

}

Handler::HANDLER_STATUS_T TweetHandler::process(TwitEng* eng, 
	std::istream& instr) const
{
	if(eng->log_sta())
	{
		string name;
		name = eng->log_user();
		string fulltext;
		string parttext;
		string text;
		string act_text;
		getline(instr, fulltext);
		// Get rid of the extra blank in front of the tweet
		stringstream ss(fulltext);
		string first_word;
		ss >> first_word;
		getline(ss,parttext);
		text = first_word + parttext;
		// Get rid of the extra blank after the tweet
		size_t text_size = text.size() - 1;
		while(isspace(text.c_str()[text_size]))
		{
			text[text_size] = '\n';
			text_size--;
		}
		stringstream text_ss(text);
		getline(text_ss, act_text);
		if(eng->exist(name))
		{
			eng->newtweet(name, act_text);
			return HANDLER_OK;
		}
		else
		{
			return HANDLER_ERROR;
		}
		return HANDLER_OK;
	}
	else
	{
		cout << "No user logged in" << endl;
		return HANDLER_ERROR;
	}
}

// Implement the function of follow handler
FollowHandler::FollowHandler()
{

}

FollowHandler::FollowHandler(Handler* next)
: Handler(next)
{

}

bool FollowHandler::canHandle(const std::string& cmd) const
{
	return cmd == "FOLLOW";
}

Handler::HANDLER_STATUS_T FollowHandler::process(TwitEng* eng, 
	std::istream& instr) const
{
	if(eng->log_sta())
	{
		string follower;
		follower = eng->log_user();
		string name_to_follow;
		instr >> name_to_follow;
		if(eng->exist(follower) && eng->exist(name_to_follow))
		{
			eng->andfollowinfo(follower, name_to_follow);
			return HANDLER_OK;
		}
		else
		{
			return HANDLER_ERROR;
		}
		return HANDLER_OK;
	}
	else
	{
		cout << "No user logged in" << endl;
		return HANDLER_ERROR;
	}
	
}

SaveHandler::SaveHandler()
{

}

SaveHandler::SaveHandler(Handler* next)
: Handler(next)
{

}

bool SaveHandler::canHandle(const std::string& cmd) const
{
	return cmd == "SAVE";
}

Handler::HANDLER_STATUS_T SaveHandler::process(TwitEng* eng, 
	std::istream& instr) const
{
	string ofile_name;
	instr >> ofile_name;
	if(!ofile_name.empty())
	{
		eng->Savehelper(ofile_name);
		return HANDLER_OK;
	}
	return HANDLER_OK;
}

SCCHandler::SCCHandler()
{

}

SCCHandler::SCCHandler(Handler* next)
: Handler(next)
{

}

bool SCCHandler::canHandle(const std::string& cmd) const
{
	return cmd == "SCC";
}

Handler::HANDLER_STATUS_T SCCHandler::process(TwitEng* eng,
	std::istream& instr) const
{
	string filename;
	instr >> filename;
	if(!filename.empty())
	{
		eng->SCC(filename);
		return HANDLER_OK;
	}
	return HANDLER_OK;
}

TrendingHandler::TrendingHandler()
{

}

TrendingHandler::TrendingHandler(Handler* next)
: Handler(next)
{

}

bool TrendingHandler::canHandle(const std::string& cmd) const
{
	return cmd == "TRENDING";
}

Handler::HANDLER_STATUS_T TrendingHandler::process(TwitEng* eng,
	std::istream& instr) const
{
	int num_item;
	instr >> num_item;
	if(instr.fail())
	{
		return HANDLER_ERROR;
	}
	if(num_item <= int(eng->hash_num()))
	{
		eng->trend_print(num_item);
		return HANDLER_OK;
	}
	return HANDLER_ERROR;
}

LoginHandler::LoginHandler()
{

}

LoginHandler::LoginHandler(Handler* next)
: Handler(next)
{

}

bool LoginHandler::canHandle(const std::string& cmd) const
{
	return cmd == "LOGIN";
}

Handler::HANDLER_STATUS_T LoginHandler::process(TwitEng* eng,
	std::istream& instr) const
{
	string user_name;
	string input_pw;
	instr >> user_name;
	if(instr.fail())
	{
		cout << "Invalid username/password." << endl;
		return HANDLER_ERROR;
	}
	if(!eng->exist(user_name))
	{
		cout << "Invalid username/password." << endl;
		return HANDLER_ERROR;
	}
	instr >> input_pw;
	if(instr.fail())
	{
		cout << "Invalid username/password." << endl;
		return HANDLER_ERROR;
	}
	if(eng->login(user_name, input_pw))
	{
		cout << "Login successful" << endl;
		return HANDLER_OK;
	}
	cout << "Invalid username/password." << endl;
	return HANDLER_ERROR;
}

LogoutHandler::LogoutHandler()
{

}

LogoutHandler::LogoutHandler(Handler* next)
: Handler(next)
{

}

bool LogoutHandler::canHandle(const std::string& cmd) const
{
	return cmd == "LOGOUT";
}

Handler::HANDLER_STATUS_T LogoutHandler::process(TwitEng* eng,
	std::istream& instr) const
{
	eng->logout();
	return HANDLER_OK;
}