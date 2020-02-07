#include "hash.h"
#include <cmath>
#include <iostream>
using namespace std;

unsigned int calculateHash(string mystring)
{
  	/* add your code here */
  	unsigned long long en_pw = 0;
  	for(size_t i = 0; i < mystring.size(); i++)
  	{
  		en_pw = (unsigned long long)(mystring[i] + 128*en_pw);
  	}
  	// declare and initialize the w
  	unsigned int w[4];
  	for(int i = 0; i < 4; i++)
  	{
  		w[i] = en_pw % 65521;
  		en_pw = en_pw/65521;
  	}
  	unsigned int answer = ((45912*w[3] + 35511*w[2] + 65169*w[1] + 4625*w[0])% 65521);
  	return answer;
}
