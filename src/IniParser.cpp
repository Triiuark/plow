#include "IniParser.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <cerrno>

#include "helper.h"
#include "PlowException.h"

using namespace std;



IniParser::IniParser(const char *iniFile)
{
  ostringstream errmsg;

  ifstream ini(iniFile);
  if(!ini)
  {
    errmsg << "Could not open " <<  iniFile;
    throw PlowException("IniParser", errmsg.str().c_str());
  }

  mSM_options = new StrMap;

  const string whitespace(" \t");

  string buffer;
  string group;
  string value;

  char *option;

  uint pos;
  uint line = 0;

  while(getline(ini, buffer))
  {
    ++line;
    value = "";

    // trim buffer
    pos = buffer.find_first_not_of(whitespace);
    if(pos != string::npos)
    {
      buffer = buffer.substr(pos);
    }
    pos = buffer.find_last_not_of(whitespace);
    if(pos != string::npos)
    {
      buffer = buffer.substr(0, pos + 1);
    }

    // ignore empty lines and comments
    if(buffer.length() == 0 || buffer.c_str()[0] == '#')
    {
      continue;
    }

    // get section
    if(buffer.c_str()[0] == '[')
    {
      if(buffer.c_str()[buffer.length() - 1] == ']')
      {
        group = buffer;
      } else {
        errmsg << "Corrupted section name in ";
        errmsg << iniFile << " at line " << line << ".";
        throw PlowException("IniParser", errmsg.str().c_str());
      }
    }

    // get option - value pairs
    else
    {
      pos = buffer.find_first_of("=");

      if(pos == string::npos)
      {
        errmsg << "Missing '=' in " << iniFile;
        errmsg << " at line " << line << ".";
        throw PlowException("IniParser", errmsg.str().c_str());
      }
      else if(pos == 0)
      {
        errmsg << "Missing option in" << iniFile;
        errmsg << " at line " << line << ".";
        throw PlowException("IniParser", errmsg.str().c_str());
      }

      value  = buffer.substr(pos + 1);
      buffer = buffer.substr(0, pos);
      pos    = buffer.find_last_not_of(whitespace);
      buffer = buffer.substr(0, pos + 1);
      pos    = value.find_first_not_of(whitespace);
      if(pos != string::npos)
      {
        value  = value.substr(pos);
      }

      if(value.length() > 0)
      {
        option = new char[group.length() + buffer.length() + 1];
        sprintf(option, "%s%s", group.c_str(), buffer.c_str());

        (*mSM_options)[option] = value;
      }
    }
  }
  ini.close();
}



string IniParser::get(const char *option)
{
  return (*mSM_options)[option];
}



IniParser::~IniParser()
{
  StrMapIt it = mSM_options->begin();

  while(it != mSM_options->end())
  {
    if(it->second != "")
    {
      //if it is empty, key wasn't created with new,
      // so I can't delete it
      delete[] it->first;
    }
    ++it;
  }

  delete mSM_options;
}
