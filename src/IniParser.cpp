/***********************************************************************
* Copyright (C) 2006 by René Bählkow                                   *
*                                                                      *
* This program is free software; you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published by *
* the Free Software Foundation; either version 2 of the License, or    *
* (at your option) any later version.                                  *
*                                                                      *
* This program is distributed in the hope that it will be useful,      *
* but WITHOUT ANY WARRANTY; without even the implied warranty of       *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU General Public License for more details.                         *
*                                                                      *
* You should have received a copy of the GNU General Public License    *
* along with this program; if not, write to the                        *
* Free Software Foundation, Inc.,                                      *
* 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.            *
***********************************************************************/

#include "IniParser.h"

#include <iostream>
#include <fstream>

#include "helper.h"
#include "PlowException.h"

using namespace std;



IniParser::IniParser(const char *iniFile) {
  err     = 0;
  options = NULL;
  string errmsg;

  ifstream ini(iniFile);
  if(!ini) {
    errmsg = "Could not open ";
    errmsg += iniFile;
    throw PlowException("IniParser", errmsg.c_str());
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
    if(it->second != "") {
      //TODO: if it is empty, maybe key wasn't created with new,
      // so I can't delete it
      delete[] it->first;
    }
    ++it;
  }

  delete options; options = NULL;
}
