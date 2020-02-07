#ifndef TWITENG_H
#define TWITENG_H
#include <map>
#include <string>
#include <set>
#include <vector>
#include <stack>
#include "user.h"
#include "datetime.h"
#include "tweet.h"
#include "heap.h"
#include "tagdata.h"

using namespace std;

class TwitEng
{
 public:
  TwitEng();
  ~TwitEng();
  /**
   * Parses the Twitter database and populates internal structures
   * @param filename of the database file
   * @return true if there is an error, false if successful
   */
  bool parse(char* filename);

  /**
   * Allocates a tweet from its parts and adds it to internal structures
   * @param username of the user who made the tweet
   * @param timestamp of the tweet
   * @param text is the actual text of the tweet as a single string
   */
  void addTweet(const std::string& username, const DateTime& time, 
    const std::string& text);

  /**
   * Searches for tweets with the given words and strategy
   * @param words is the hashtag terms in any case without the '#'
   * @param strategy 0=AND, 1=OR
   * @return the tweets that match the search
   */
  std::vector<Tweet*> search(std::vector<std::string>& terms, int strategy);

  std::vector<Tweet*> ANDsearch(std::vector<std::string>& terms);

  std::vector<Tweet*> ORsearch(std::vector<std::string>& terms);

  std::set<Tweet*> intersect(const std::set<Tweet*>& s1, 
                                const std::set<Tweet*>& s2);
  std::set<Tweet*> uni(const std::set<Tweet*>& s1, 
                                const std::set<Tweet*>& s2);
  bool exist(const string name);
  void newtweet(string name, string text);
  void andfollowinfo(string follower, string name_to_follow);
  void Savehelper(string filename);
  /**
   * Dump feeds of each user to their own file
   */

  void dumpFeeds();

  // To collect the mention info and store them into the mention feed
  void mentioninfo();

  // Strong connected components function
  void SCC(string filename);

  // Strong connected components helper function
  void SCC_helper(User* user, int& index, stack<User*>& user_s, 
    set<User*>& visited, int& scc_index);
  void SCC_print(string filename);
  void trend_print(int n);
  size_t hash_num();
  bool login(string user_name, string password);
  bool log_sta();
  string log_user();
  void logout();
  /* You may add other member functions */
 private:
  /* Add any other data members or helper functions here  */
  int num;
  // a map store all users info
  map< string, User* > all_users;
  // a map store the hashtag and corresponding tweets
  map< string, set<Tweet*> > hashtwi;
  // a map store the name and the corresponding mentioned tweets
  map< string, vector<Tweet*> > mentionmap;
  // a map to store the scc relation
  map< int, vector<User*> > scc_map;
  // a heap to store the hash and its corresponding tweets
  Heap<TagData, TagStringEqual, TagIntGreater, TagStringHasher> trending_;
  // store the password for each user
  map<string, unsigned int> pw_map;
  string curr_user;
  bool log_status;
};


#endif