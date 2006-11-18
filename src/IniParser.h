
#ifndef PLOW_INI_PARSER_H
#define PLOW_INI_PARSER_H

#include "global.h"

using namespace std;

class IniParser {
  public:
    IniParser(const char *iniFile);

    string get(const char *option);
    int   error();

    ~IniParser();

  private:
    int err;
    a_array_s *options;

};

#endif
