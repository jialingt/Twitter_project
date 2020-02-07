all: twitter

twitter: twitter.cpp twiteng.o util.o user.o tweet.o hsort.h\
	cmdhandler.o handler.o datetime.o hash.o
	g++ -g -Wall -std=c++11 twitter.cpp twiteng.o user.o \
	handler.o util.o tweet.o cmdhandler.o datetime.o hash.o -o twitter

twiteng.o: twiteng.h twiteng.cpp util.h user.h datetime.h tweet.h\
	tagdata.h heap.h hash.h
	g++ -g -Wall -std=c++11 -c twiteng.cpp -o twiteng.o

util.o: util.h tweet.h util.cpp
	g++ -g -Wall -std=c++11 -c util.cpp -o util.o

datetime.o: datetime.h datetime.cpp
	g++ -g -Wall -std=c++11 -c datetime.cpp -o datetime.o

tweet.o: tweet.h datetime.h tweet.cpp user.h
	g++ -g -Wall -std=c++11 -c tweet.cpp -o tweet.o

user.o: user.cpp user.h tweet.h
	g++ -g -Wall -std=c++11 -c user.cpp -o user.o

cmdhandler.o: cmdhandler.h cmdhandler.cpp util.h twiteng.h handler.h
	g++ -g -Wall -std=c++11 -c cmdhandler.cpp -o cmdhandler.o

handler.o: handler.cpp handler.h twiteng.h
	g++ -g -Wall -std=c++11 -c handler.cpp -o handler.o

hash.o: hash.cpp hash.h
	g++ -g -Wall -std=c++11 -c hash.cpp -o hash.o