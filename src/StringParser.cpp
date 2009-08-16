#include "StringParser.h"

#include <iostream>
#include <string>

#include <cstring>

using namespace std;

StringParser::StringParser(const char *str, const char delim)
{
	mArgv = 0;

	string mystring(str);
	string *token = 0;
	size_t pos;

	while(!mystring.empty()) {
		if(mystring.c_str()[0] != '"') {
			/// token is from beginning of mystring up to first ' ' or
			/// end of mystring
			pos = mystring.find_first_of(delim);
			token = new string(mystring);

			if(pos < string::npos) {
				token->resize(pos);
				mystring = mystring.substr(pos + 1);
			} else {
				mystring.clear();
			}
		} else {
			/// remove first '"'
			mystring = mystring.substr(1);
			/// find next '"'
			pos = mystring.find_first_of('"');
			if(pos != string::npos) {
				/// mystring is not complete quoted
				/// put everything quoted into token and ...
				token = new string(mystring.substr(0, pos));
				/// ... remove it from mystring
				mystring = mystring.substr(pos + 1);
			} else {
				/// complete mystring is quoted, so put everything in token
				token = new string(mystring);
				mystring.clear();
			}
		}
		if(token && !token->empty()) {
			mTokens.push_back(token);
		} else if(token) {
			/// clear unaccessary ones
			delete token;
		}
	}
}

vector<string *> const& StringParser::getTokens() const
{
	return mTokens;
}

unsigned int StringParser::getSize() const
{
	return mTokens.size();
}

char **StringParser::getArgv()
{
	if(mArgv == 0) {
		unsigned int size = mTokens.size();
		mArgv = new char*[size + 1];
		memset(mArgv, 0, sizeof(char *) * (size + 1));
		for(unsigned int i = 0; i < size; ++i) {
			mArgv[i] = (char *)(mTokens[i]->c_str());
		}
	}

	return mArgv;
}

StringParser::~StringParser()
{
	if(mArgv) {
		delete[] mArgv;
	}

	for(unsigned int i = 0; i < mTokens.size(); ++i) {
		delete mTokens[i];
	}
}
