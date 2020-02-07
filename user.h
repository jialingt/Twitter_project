#ifndef USER_H
#define USER_H

#include <string>
#include <set>
#include <list>
#include <vector>
#include "tweet.h"

using namespace std;

/* Forward Declaration to avoid #include dependencies */
class Tweet;

class User {
 public:
  /**
   * Constructor 
   */
  User(std::string name);

  /**
   * Destructor
   */
  ~User();

  /**
   * Gets the name of the user 
   * 
   * @return name of the user 
   */
  std::string name() const; 

  /**
   * Gets all the followers of this user  
   * 
   * @return Set of Users who follow this user
   */
  std::set<User*> followers() const;

  /**
   * Gets all the users whom this user follows  
   * 
   * @return Set of Users whom this user follows
   */
  std::set<User*> following() const;

  /**
   * Gets all the tweets this user has posted
   * 
   * @return List of tweets this user has posted
   */
  std::list<Tweet*> tweets() const; 

  /**
   * Adds a follower to this users set of followers
   * 
   * @param u User to add as a follower
   */
  void addFollower(User* u);

  /**
   * Adds another user to the set whom this User follows
   * 
   * @param u User that the user will now follow
   */
  void addFollowing(User* u);

  /**
   * Adds the given tweet as a post from this user
   * 
   * @param t new Tweet posted by this user
   */
  void addTweet(Tweet* t);

  /**
   * Produces the list of Tweets that represent this users feed/timeline
   *  It should contain in timestamp order all the tweets from
   *  this user and all the tweets from all the users whom this user follows
   *
   * @return vector of pointers to all the tweets from this user
   *         and those they follow in timestamp order
   */
  std::vector<Tweet*> getFeed();

  int index() const;

  int lowlink() const;

  int& index();

  int& lowlink();

  /* You may add other member functions */
 private:
  /* Add any other data members or helper functions here  */
  std::string name_;
  std::set<User*> follower_;
  std::set<User*> following_;
  std::list<Tweet*> tweets_;
  int idx;
  int lowlk;
};

#endif
