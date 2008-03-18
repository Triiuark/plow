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

#define PACKAGE "plow"
#ifndef VERSION
#define VERSION "(version not set)"
#endif

#include <iostream>
#include <sstream>
#include <string>

extern "C"
{
#include <sqlite3.h> /// for quoting with sqlite3_mprintf()
}

#include "constants.h"

#include "PlowException.h"
#include "Plow.h"

using namespace std;

/**
 * prints out a help message
 */
void printUsage()
{
  cout << USAGE << "\n\n" << HELP << endl;
}



/**
 * prints out version
 */
void printVersion()
{
  cout << PACKAGE << " " << VERSION << endl;
}



/**
 *  for parsing of --set option
 */
int parseUpdate(int actual, int argc, char *argv[], Plow &plow)
{
  char key;
  const char *field;

  ostringstream errmsg;

  while(actual < argc
        && argv[actual][0] != '-'
        && argv[actual][0] != '+')
  {
    if(argv[actual][1] != 0)
    {
      errmsg << "wrong syntax near '" << argv[actual] << "'";
      throw PlowException("paseUpdate", errmsg.str().c_str());
    }

    key   = argv[actual][0];
    field = Plow::getField(key)->first;

    if(field == 0)
    {
      errmsg << "unkown field '" << key << "'";
      throw PlowException("parseUpdate", errmsg.str().c_str(), USAGE);
    }

    if(argc < actual + 2) {
      errmsg << "missing argument for field '" << key
             << " (" << field << ")'";
      throw PlowException("parseUpdate", errmsg.str().c_str(), USAGE);
    }

    ++actual;
    plow.appendUpdate(key, argv[actual]);
    ++actual;
  }

  return actual - 1;
}



/**
 *  for parsing of filter
 */
int parseFilter(int actual, int argc, char **argv, Plow &plow)
{
  char key = argv[actual][1];

  const char *field = Plow::getField(key)->first;

  ostringstream errmsg;

  if(field == 0)
  {
    errmsg << "unkown field '" << key << "'";
    throw PlowException("parseFilter", errmsg.str().c_str(), USAGE);
  }

  if(argc < actual + 2)
  {
    errmsg << "missing argument for field '" << key << "'";
    throw PlowException("parseFilter", errmsg.str().c_str(), USAGE);
  }

  string pre;
  string post = "' OR ";

  if(argv[actual][2] == 0)
  {
    pre  = " LIKE '%%";
    post = "%%' OR ";
  }
  else if(argv[actual][2] == 'e' && argv[actual][3] == 0)
  {
    pre  = "='";
  }
  else if(argv[actual][2] == 'l' && argv[actual][3] == 0)
  {
    pre  = "<'";
  }
  else if(argv[actual][2] == 'g' && argv[actual][3] == 0)
  {
    pre  = ">'";
  }
  else
  {
    errmsg << "wrong syntax near '" << argv[actual] << "'";
    throw PlowException("parseFilter", errmsg.str().c_str(), USAGE);
  }


  string filter;

  if(argv[actual][0] == '+')
  {
    filter.append("\tAND NOT (");
  }
  else
  {
    filter.append("\tAND (");
  }

  int pos = ++actual;
  char *quoted;

  while(actual < argc
        && (argv[actual][0] != '-' && argv[actual][0] != '+'))
  {
    quoted = sqlite3_mprintf("%q", argv[actual]);

    filter.append(field);
    filter.append(pre);
    filter.append(quoted);
    filter.append(post);

    sqlite3_free(quoted);

    ++actual;
  }

  if(pos == actual)
  {
    errmsg << "Missing argument for option '" << key << "'";
    throw PlowException("parseFilter", errmsg.str().c_str(), USAGE);
  }

  /// replace last " OR "
  filter.replace(filter.length() - 4, 4, ")\n");

  plow.appendFilter(filter);

  return actual - 1;
}



/**
 *  parses all options starting with '--'
 */
int parseLongOption(int actual, int argc, char *argv[], Plow &plow)
{
  char *value = &argv[actual][2];

  if(strcmp(value, "v") == 0 || strcmp(value, "version") == 0)
  {
    printVersion();

    exit(0);
  }

  else if(strcmp(value, "h") == 0 || strcmp(value, "help") == 0)
  {
    printUsage();

    exit(0);
  }

  else if(strcmp(value, "q") == 0 || strcmp(value, "query") == 0)
  {
    if(argv[++actual] == 0)
    {
      throw PlowException("parseLongOption",
                          "missing sql statement for option '--query'",
                          USAGE);
    }

    plow.setQuery(argv[actual++]);
  }

  else if(strcmp(value, "r") == 0 || strcmp(value, "random") == 0)
  {
    plow.setRandom(true);
  }

  else if(strcmp(value, "a") == 0 || strcmp(value, "add") == 0)
  {
    plow.setAppend(true);
  }

  else if(strcmp(value, "n") == 0 || strcmp(value, "noplay") == 0)
  {
    plow.setPlay(false);
  }

  else if(strcmp(value, "s") == 0 || strcmp(value, "show") == 0)
  {
    plow.setShowQuery(true);
  }

  else if(strcmp(value, "c") == 0 || strcmp(value, "copy") == 0)
  {
    plow.setCopy(true);
  }

  else if(value[1] == 0 && value[0] > 47 && value[0] < 58)
  {
    plow.setPlayer(value[0]);
  }

  else if(strcmp(value, "l") == 0 || strcmp(value, "list") == 0)
  {
    if(argv[++actual] != 0)
    {
      char *quotedValue = sqlite3_mprintf(argv[actual]);
      char query[42 + 4 * strlen(quotedValue)];

      sprintf(query, "SELECT id_%s, %s FROM tbl_%s ORDER BY %s;",
              quotedValue, quotedValue, quotedValue, quotedValue);

      plow.setQuery(query);

      sqlite3_free(quotedValue);
    }
    else
    {
      throw PlowException("parseArguments",
                          "missing argument for option '--print'",
                          USAGE);
    }
  }

  else if(strcmp(value, "S") == 0 || strcmp(value, "set") == 0)
  {
    actual = parseUpdate(++actual, argc, argv, plow);
  }

  else if(strcmp(value, "i") == 0 || strcmp(value, "insert") == 0)
  {
    if(actual + 1 < argc)
    {
      plow.insert(argv[++actual]);
      ++actual;
    }
    else
    {
      throw PlowException("parseArgs",
                          "missing argument for option '--insert'",
                          USAGE);
    }
  }

  else
  {
    ostringstream errmsg;
    errmsg << "unkown option '" << value << "'";
    throw PlowException("parseLongOption", errmsg.str().c_str(), USAGE);
  }

  return actual;
}



/**
 * parses arguments and calls the appropriate parser function
 */
void parseArguments(int argc, char *argv[], Plow &plow)
{
  for(int i = 1; i < argc; ++i)
  {
    switch(argv[i][0])
    {
      case '-':
        switch(argv[i][1])
        {
          case '-':
            i = parseLongOption(i, argc, argv, plow);
          break;

          default:
            i = parseFilter(i, argc, argv, plow);
          break;
        }
      break;

      case '+':
        i = parseFilter(i, argc, argv, plow);
      break;

      default:
        ostringstream errmsg;
        errmsg << "wrong syntax near '" << argv[i] << "'";
        throw PlowException("parseArguments", errmsg.str().c_str(), USAGE);
    }
  }
}



/**
 * hmm
 */
int main(int argc, char *argv[])
{
  Plow plow;

  try
  {
    parseArguments(argc, argv, plow);

    plow.run();
  }

  catch(PlowException &e)
  {
    e.print();
    return 1;
  }

  catch(string &str)
  {
    cerr << "Exception: " << str << endl;
    cout << endl;
    return 1;
  }

  catch(...)
  {
    cerr << "Unhandled exception" << endl;
    cout << endl;
    return 1;
  }

  return 0;
}
