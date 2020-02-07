#include <iostream>
#include <sstream>
#include <string>
#include "twiteng.h"
#include "util.h"
#include "tweet.h"
#include "cmdhandler.h"

using namespace std;

Handler* createHandlers();

Handler* createHandlers()
{
  return new AndHandler(
    new OrHandler(
      new TweetHandler(
        new FollowHandler(
          new SaveHandler(
            new QuitHandler(
              new SCCHandler(
                new TrendingHandler(
                  new LoginHandler(
                    new LogoutHandler(
                    )
                  )
                )
              )
            )
          )
        )
      )
    )
  );
}

int main(int argc, char* argv[])
{
  if(argc < 2){
    cerr << "Please provide the twitter data file" << endl;
    return 1;
  }
  TwitEng twit;
  if ( twit.parse(argv[1]) ){
    cerr << "Unable to parse " << argv[1] << endl;
    return 1;
  }

  // Initialize command handlers
  Handler* handlers = createHandlers();

 cout << "=====================================" << endl;
  cout << "Menu: " << endl;
  cout << "  LOGIN username password            " << endl;
  cout << "  LOGOUT                             " << endl;
  cout << "  TRENDING n (n=top n hashtags)      " << endl;
  cout << "  AND term term ...                  " << endl;
  cout << "  OR term term ...                   " << endl;
  cout << "  TWEET tweet_text" << endl;
  cout << "  FOLLOW user_to_follow              " << endl;
  cout << "  SAVE new_filename                  " << endl;
  cout << "  QUIT (and write feed files)        " << endl;
  cout << "=====================================" << endl;

  Handler::HANDLER_STATUS_T status = Handler::HANDLER_OK;
  while(status != Handler::HANDLER_QUIT && 
        status != Handler::HANDLER_KILL)
  {
    cout << "\nEnter command: " << endl;
    string line;
    getline(cin,line);
    stringstream ss(line);
    string cmd;
    if((ss >> cmd)){
      // convert to upper case
      convUpper(cmd); 
      // Invoke the chain of responsibility
      status = handlers->handle(&twit, cmd, ss);
    }
  }
  delete handlers;
  return 0;
}
