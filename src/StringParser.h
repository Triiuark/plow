
#ifndef PLOW_STRING_PARSER_H
#define PLOW_STRING_PARSER_H

#include <string>
#include <vector>

using namespace std;

class StringParser {
  public:
    StringParser(const char *str, const char delim = ' ');

    vector<string *> getTokens();

    char **getArgv();
    uint   getSize();

    ~StringParser();
  private:
    char **argv;
    vector<string *> tokens;
};

#endif
