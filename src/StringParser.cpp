
#include "StringParser.h"
#include <iostream>
#include <string>

using namespace std;

StringParser::StringParser(const char *str, const char delim) {
  string  mystring(str);

  string *token = NULL;
          argv  = NULL;
  uint    pos;

  while(!mystring.empty()) {
    if(mystring.c_str()[0] != '"') {
      // token goes from beginning of mystring up to first ' ' or
      // end of mystring
      pos = mystring.find_first_of(delim);
      token = new string(mystring);
      if(pos < string::npos) {
        token->resize(pos);
        mystring = mystring.substr(pos + 1);
      } else {
          mystring.clear();
      }
    } else {
      // remove first '"'
      mystring = mystring.substr(1);
      // find next '"'
      pos = mystring.find_first_of('"');
      if(pos != string::npos) {
        // mystring is not complete quoted
        // put everything quoted into token and ...
        token = new string(mystring.substr(0, pos));
        // ... remove it from mystring
        mystring = mystring.substr(pos + 1);
      } else {
        // complete mystring is quoted, so put everything in token
        token = new string(mystring);
        mystring.clear();
      }
    }
    if(token && !token->empty()) {
      tokens.push_back(token);
    } else if(token) {
      // clear unaccessary ones
      delete token;
    }
  }
}

vector <string *>StringParser::getTokens() {
  return tokens;
}


uint StringParser::getSize() {
  return tokens.size();
}

/* returns a char ** like argv ;) of the tokens
 * but it is NULL terminated so we know where it ends
 * (for use in execvp)
 */
char **StringParser::getArgv() {
  uint size = tokens.size();
       argv = new char*[size + 1];

  memset(argv, 0, sizeof(char *) * (size + 1));

  for(uint i = 0; i < size; i++) {
    argv[i] = (char *)(tokens[i]->c_str());
  }

  return argv;
}

StringParser::~StringParser() {
  for(uint i = 0; i < tokens.size(); i++) {
    delete tokens[i]; tokens[i] = NULL;
  }
  delete[] argv; argv = NULL;
}
