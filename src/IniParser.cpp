
#include "IniParser.h"

#include <iostream>
#include <fstream>

#include "helper.h"

using namespace std;

IniParser::IniParser(const char *iniFile) {
  err     = 0;
  options = NULL;

  ifstream ini(iniFile);

  if(!ini) {
    err = 3;
    return;
  }

  options = new a_array_s;

  uint length = 1024; //TODO: search for a runtime solution

  char buffer[length];
  char group [length];

  char *option;
  char *value;

  while(ini.getline(buffer, length)) {
    if(!strlen(buffer) || buffer[0] == '#') {
      continue;
    }
    if(buffer[0] == '[') {
      if(buffer[strlen(buffer) - 1] == ']') {
        sprintf(group, "%s", buffer);
      } else {
        err = 1;
        break;
      }
    } else {
      value = index(buffer, '=');
      if(!value) {
        err = 2;
        continue;
      }

      value[0] = 0; // buffer = 'OPTION=VALUE'
                    // remove '=VALUE' from buffer

      option = new char[strlen(group) + strlen(buffer) + 1];
      sprintf(option, "%s%s", group, buffer);

      (*options)[option] = &value[1]; // value without '='
    }
  }

  ini.close();
}

string IniParser::get(const char *option) {
  return (*options)[option];
}

int IniParser::error() {
  return err;
}

IniParser::~IniParser() {

  a_array_s_it it = options->begin();

  while(it != options->end()) {
    delete[] it->first;
    ++it;
  }

  delete options; options = NULL;

}
