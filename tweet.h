#ifndef TWEET_H
#define TWEET_H
#include <iostream>
#include <string>
#include <string>
#include <vector>
#include <set>
#include "datetime.h"
#include "user.h"

/* Forward declaration */
class User;

/**
 * Models a tweet and provide comparison and output operators
 */
class Tweet
{
 public:
  /**
   * Default constructor 
   */
  Tweet();

  /**
   * Constructor 
   */
  Tweet(User* user, const DateTime& time, const std::string& text);

  /**
   * Gets the timestamp of this tweet
   *
   * @return timestamp of the tweet
   */
  DateTime const & time() const;

  /**
   * Gets the actual text of this tweet
   *
   * @return text of the tweet
   */
  std::string const & text() const;

  /**
   * Returns the hashtagged words without the '#' sign
   *
   */
  std::set<std::string> hashTags() const;

  /**
   * Return true if this Tweet's timestamp is less-than other's
   *
   * @return result of less-than comparison of tweet's timestamp
   */
  bool operator<(const Tweet& other) const; 

  /**
   * Outputs the tweet to the given ostream in format:
   *   YYYY-MM-DD HH::MM::SS username tweet_text
   *
   * @return the ostream passed in as an argument
   */
  friend std::ostream& operator<<(std::ostream& os, const Tweet& t);

  /* Create any other public or private helper functions you deem 
     necessary */

  User* user() const;

  /* You may add other member functions */
 private:
  /* Add any other data members or helper functions here  */
  User* user_;
  DateTime time_;
  std::string text_;
  std::set<std::string> hashtag_;


};

/***********************************/
/* Leave this alone --             */
/*   it will help sort your tweets */
/***********************************/
struct TweetComp
{
  bool operator()(Tweet* t1, Tweet* t2)
  {
    return (*t2 < *t1);
  }
};
#endif
