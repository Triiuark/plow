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
#define VERSION "unknown version"
#endif

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>
#include <queue>

#include <cstring>
#include <cerrno>

#include <sqlite3.h>

#include <sys/stat.h>

#include "constants.h"
#include "global.h"
#include "helper.h"

#include "PlowException.h"
#include "IniParser.h"
#include "Sqlite3.h"
#include "Sqlite3Result.h"
#include "StringParser.h"
#include "TagReader.h"

#include <iconv.h>

using namespace std;

IniParser *gIniParser = 0;

string gsPlowHome;
string gsSelect;
string gsUpdate;
string gsSet;
string gsDatabase;
string gsMusicDirectory;
string gsPlaylist;


char gcPlayerNumber  = '0';   // player number
                              // -0 | -1 | ... | -9
bool gbShowquery     = false; // print out the query
                              // -Q
bool gbPlay          = true;  // start playing after all
                              // --noplay = do not play
bool gbAddToPlaylist = false; // add the query to current playlist
                              // --add
bool gbShuffle       = false; // shuffle playlist
                              // -S


/**
 * adds files in path into database dbPath, and strips musicPath from
 * file names
 */
void add2Db(char *path, const char *dbPath, const char *musicPath);



/**
 * prints out a help message
 */
void printusage()
{
  cout << USAGE << endl;
  cout << endl;
  cout << HELP << endl;
} // printusage()



/**
 *
 * executes a sql query
 * @param query a sql statement
 *
 * @returns a new Sqlite3Result object for @a query
 */
Sqlite3Result *exe(const char *query)
{
  Sqlite3 sql(gsDatabase.c_str());
  Sqlite3Result *rs = sql.exe(query);

  return rs;
} // exe()




/**
 * create missing files and gIniParser
 */
void init()
{
  if(!gIniParser) {
    struct stat st;
    if(stat(gsPlowHome.c_str(), &st) == -1) {
      if(errno == ENOENT) {
        mkdir_r(gsPlowHome);
        cout << "> created directory:   " << gsPlowHome << "/" << endl;
      }
    }

    string inifile = gsPlowHome + "/plow.conf";

    if(stat(gsDatabase.c_str(), &st) == -1) {
      if(errno == ENOENT) {
        exe(DATABASE);
        cout << "> created database:    " << gsDatabase << endl;
      }
    }

    if(stat(inifile.c_str(), &st) == -1) {
      if(errno == ENOENT) {
        ofstream fout(inifile.c_str());
        fout << INI_FILE;
        fout.close();

        cout << "> created config file: " << inifile << endl;
        cout << endl;
        cout << "You have to edit " << inifile << " now." << endl;
        exit(0);
      }
    }

    gIniParser       = new IniParser(inifile.c_str());
    gsMusicDirectory = gIniParser->get("[general]path");

    if(!gIniParser->get("[general]playlist").empty()) {
      gsPlaylist = gIniParser->get("[general]playlist");
    }

  }
} // init()



/**
 * get the required fields for the filename
 */
string infoString(Sqlite3Result &rs,
                  uint row,
                  const vector<string *> &tokens,
                  bool removeslash = false)
{
  const string  forbidden = "\\\n\r\" '$@*{}[]()/:;&?";

  string        out;
  string        tmp;
  string        tmp2;

  uint strsize = 0;

  bool isField   = true;
  bool lastEmpty = false;

  for(uint i = 0; i < tokens.size(); i++) {
    tmp = "";
    isField = true;

    strsize = tokens[i]->size();

    //
    // first some 'special' fields
    //
    if(tokens[i]->c_str()[0] == '['
        && tokens[i]->c_str()[strsize - 1] == ']') {
      if(*tokens[i] == "[track0]") {
        if(strlen(rs.get(row, "track")) < 2) {
          tmp += "0";
        }
        tmp += rs.get(row, "track");
      } else if(*tokens[i] == "[tracks0]") {
        if(strlen(rs.get(row, "tracks")) < 2) {
          tmp += "0";
        }
        tmp += rs.get(row, "tracks");
      } else if(*tokens[i] == "[part0]") {
        if(strlen(rs.get(row, "part")) < 2) {
          tmp += "0";
        }
        tmp += rs.get(row, "part");
      } else if(*tokens[i] == "[parts0]") {
        if(strlen(rs.get(row, "parts")) < 2) {
          tmp += "0";
        }
        tmp += rs.get(row, "parts");
      } else if(*tokens[i] == "[fileext]") {
        tmp2 = rs.get(row, "file");
        uint pos = tmp2.rfind('.');
        if(pos != string::npos) {
          tmp += tmp2.substr(pos);
        }
      } else if(*tokens[i] == "[artistOrAlbum]") {
        if(strcmp(rs.get(row, "album_id_artist"), "1") != 0) {
          tmp += rs.get(row, "artist");
        } else {
          tmp += rs.get(row, "album");
        }
      } else if(*tokens[i] == "[albumOrArtist]") {
        if(strcmp(rs.get(row, "album_id_artist"), "1") != 0) {
          tmp += rs.get(row, "album");
        } else {
          tmp += rs.get(row, "artist");
        }
      } else if(*tokens[i] == "[albumOrEmpty]") {
        if(strcmp(rs.get(row, "album_id_artist"), "1") != 0) {
          tmp += rs.get(row, "album");
        } else {
          lastEmpty = true;
        }
      } else if(*tokens[i] == "[emptyOrAlbum]") {
        if(strcmp(rs.get(row, "album_id_artist"), "1") == 0) {
          tmp += rs.get(row, "album");
        } else {
          lastEmpty = true;
        }
      } else if(*tokens[i] == "[artistOrEmpty]") {
        if(strcmp(rs.get(row, "album_id_artist"), "1") != 0) {
          tmp += rs.get(row, "artist");
        } else {
          lastEmpty = true;
        }
      } else if(*tokens[i] == "[emptyOrArtist]") {
        if(strcmp(rs.get(row, "album_id_artist"), "1") == 0) {
          tmp += rs.get(row, "artist");
        } else {
          lastEmpty = true;
        }
      }

      //
      // now fields from database
      //
      else {
        tmp2 = &(tokens[i]->c_str()[1]);
        tmp2.erase(tmp2.size() - 1, 1);
        if(rs.get(row, tmp2.c_str()) != NULL) {
          tmp += rs.get(row, tmp2.c_str());
        } else {
          isField = false;
          tmp += *tokens[i];
        }
      }
    } else if (!lastEmpty) {
      isField = false;
      tmp += *tokens[i];
    } else if (lastEmpty) {
      lastEmpty = false;
    }

    if(removeslash && isField) {
      // create a legal file name
      replaceChars(forbidden, tmp);
      out.append(tmp);
    } else {
      out += tmp;
    }
  }

  return out;
} // infoString()



/**
 * copies files in playlist to the portable device
 */
void copy2Portable()
{
  vector<string> *files = new vector<string>;

  string portable = gIniParser->get("[general]portable");

  if(portable.empty()) {
    delete files;
    errno = 0;
    throw PlowException("copy2Portable",
                        "no portable device set in config file");
  }

  cout << "Copying " << flush;

  //
  // read playlist and add files to files
  //

  ifstream inputStream;
  inputStream.open(gsPlaylist.c_str(), ios::in);
  if(!inputStream) {
    delete files;
    throw PlowException("copy2Portable", gsPlaylist.c_str());
  }

  char *buf = new char[1024];

  int numFiles = 0;
  while(inputStream.getline(buf, 1024)) {
    if(buf[0] != '#') {
      numFiles++;
      files->push_back(buf);
    }
  }
  inputStream.close();
  delete[] buf;


  cout << numFiles <<  " files to " << portable << " ..." << endl;

  string dst;

  //
  // find out how to rename files and copy them
  //

  string portableName = gIniParser->get("[general]portable_name");

  string query(SELECT_ALL);
  query.append(WHERE);
  query.append(" AND file='");

  string q;

  Sqlite3Result *rs;

  for(uint i = 0, j = files->size(); i < files->size(); i++, j--) {
    dst = portable + "/";

    if(!portableName.empty()) {
      q  = query + files->at(i).substr(gsMusicDirectory.size()) + "';";
      rs = exe(q.c_str());

      if(rs->cols() == 0) {
        string *tmp = new string("file not found in database: ");
        tmp->append(files->at(i));
        errno = 0;
        PlowException *e = new PlowException("copy2Portable",
                                             tmp->c_str());
        e->print();
        delete rs;
        delete tmp;
        delete e;
        continue;
      }

      dst += infoString(*rs,
                        0,
                        StringParser(portableName.c_str()).getTokens(),
                        true);
      delete rs;
    } else {
      // fallback: use original filename
      dst += files->at(i).substr(gsMusicDirectory.size());
    }

    try {
      if(copyfile(files->at(i), dst) == 1) {
        // file already exists
        cout << "*";
      }
      cout << j << " " << flush;
    } catch (PlowException &e) {
      e.print();
      if(e.error() == ENOSPC || e.error() == ENOTDIR) {
        // no space left on device or can't create dir -> break here
        break;
      }
    }
  }

  delete files;

  cout << "\n... done" << endl;

} //copy2Portable()



/**
 * executes query and prints out the result as a nice table
 *
 * @param query a sql statement
 */
void printResult(const char *query)
{
  Sqlite3Result *rs = exe(query);

  if(rs->cols()) {
    cout << "| ";
    int len = 0;
    for(int i = 0; i < rs->cols(); ++i) {
      len = strlen(rs->getHead(i)) - utf8strlen(rs->getHead(i));
      cout << setw(rs->getWidth(i) + len);
      cout << setiosflags(ios::left);
      cout << rs->getHead(i);
      cout << " | ";
    }
    cout << endl;
    cout << endl;

    for(int i = 0; i < rs->rows(); i++) {
      cout << "| ";
      for(int j = 0; j < rs->cols(); j++) {
        len = strlen(rs->get(i, j)) - utf8strlen(rs->get(i, j));
        cout << setw(rs->getWidth(j) + len);
        cout << setiosflags(ios::left) << rs->get(i, j);
        cout << " | ";
      }
      cout << endl;
    }
  } else {
    cout << "Successfully executed satement. Empty result." << endl;
  }
  delete rs;
} // printResult()



/**
 * creates a playlist for the given query
 *
 * @param query a sql statement
*/
void createList(const char *query)
{
  Sqlite3Result *rs = exe(query);

  if(rs->rows() == 0) {
    gbPlay = false;
    cout << "Empty result. Sorry." << endl;
    delete rs;
    return;
  } else if(rs->rows() == 1) {
    cout << "1 file found";
  } else {
    cout << rs->rows() << " files found";
  }

  _Ios_Openmode open_mode=ios::out;
  if(gbAddToPlaylist) {
    open_mode=ios::app;
  }
  ofstream m3u(gsPlaylist.c_str(), open_mode);
  if(!m3u) {
    throw PlowException("createList", "Can't open playlist file.");
  }

  if(!gbAddToPlaylist) {
    m3u << "#EXTM3U" << endl;
  }

  uint playtime = 0;
  uint total_playtime = 0;
  StringParser sp(gIniParser->get("[general]extinf").c_str());
  bool extinf = true;
  if(gIniParser->get("[general]extinf").empty()) {
    extinf = false;
  }

  for(int i = 0; i < rs->rows(); ++i) {
    playtime = (uint)(atof(rs->get(i, "length")));
    total_playtime += playtime;
    if(extinf) {
      m3u << "#EXTINF:" << infoString(*rs, i, sp.getTokens()) << endl;
    }
    m3u << rs->get(i, 0) << endl;
  }
  m3u.close();

  uint hours = (total_playtime / 3600);
  uint mins  = (total_playtime - (hours * 3600)) / 60;
  uint secs  = (total_playtime - (hours * 3600)) - (mins * 60);
  cout << " (" << hours << "h" << mins << "m" << secs << "s)." << endl;

  delete rs;
} // createList()



/**
 * parses the command-line arguments for the set options
 * (creates gsUpdate)
 *
 * @param argc number of arguments
 * @param argv arguments
 * @param i    number of the argument to parse
 *
 * @throws PlowException on any error
 *
 * @return number of the next argument to parse
 */
int parseSetOptions(int argc, char **argv, int i)
{
  map<char, string> fieldNames;

  fieldNames['a'] = "artist";
  fieldNames['r'] = "rating";
  fieldNames['g'] = "genre";
  fieldNames['l'] = "language";
  fieldNames['m'] = "mood";
  fieldNames['A'] = "album";
  fieldNames['s'] = "situation";
  fieldNames['t'] = "tempo";

  ostringstream errmsg;

  string select;
  string insert;
  string in;

  char       f      = 0;
  char       *value = 0;
  const char *id    = 0;

  Sqlite3Result *result = 0;

  while( i < argc && argv[i][0] != '-') {
    f = argv[i][0];

    if(fieldNames[f] == "") {
      errmsg << "Missing option near '" << argv[i] << "'";
      throw PlowException("parseSetOptions", errmsg.str().c_str(), USAGE);
    }
    if(argc < i + 2) {
      errmsg << "Missing argument for option '" << f << "'";
      throw PlowException("parseSetOptions", errmsg.str().c_str(), USAGE);
    }

    value = sqlite3_mprintf("'%q'", argv[i+1]);

    select  = "SELECT * ";
    select += "FROM tbl_";
    select += fieldNames[f];
    select += " WHERE ";
    select += fieldNames[f];
    select += "=";
    select += value;
    select += ";";


    result = exe(select.c_str());

    if(!result->rows()) {
      delete result;
      cout << "> There is no " << fieldNames[f];
      cout << " '" << argv[i+1] << "'" << endl;
      cout << "> Do you want to create it? y/[n] ";
      cin >> in;
      if(in[0] == 'y') {
        insert  = "INSERT INTO tbl_";
        insert += fieldNames[f];
        insert += " (";
        insert += fieldNames[f];
        insert += ") VALUES (";
        insert += value;
        insert += ");";
        result = exe(insert.c_str());
        delete result;
        result = exe(select.c_str());
      } else {
        cout << "> aborted ..." << endl;
        return i+1;
      }

    }

    id = result->get(0, 0);

    gsUpdate += ", _id_";
    gsUpdate += fieldNames[f];
    gsUpdate += "=";
    gsUpdate += id;

    delete result;

    i+=2;
  }

  return i-1;
} // parseSetOptions()



/**
 * parses the command-line arguments for filtering
 * (creates gsSelect)
 *
 * @param argc number of arguments
 * @param argv arguments
 * @param i    number of the argument to parse
 *
 * @throws PlowException on any error
 *
 * @return number of the next argument to parse
 */
int parseFilter(int argc, char **argv, int i)
{
  map<char, string> fieldNames;

  fieldNames['T'] = "title";
  fieldNames['a'] = "artist";
  fieldNames['r'] = "rating";
  fieldNames['g'] = "genre";
  fieldNames['l'] = "language";
  fieldNames['m'] = "mood";
  fieldNames['A'] = "album";
  fieldNames['s'] = "situation";
  fieldNames['t'] = "tempo";

  char f = argv[i][1];

  ostringstream errmsg;

  if(fieldNames[f] == "") {
    errmsg << "Missing option near '" << argv[i] << "'";
    throw PlowException("parseFilter", errmsg.str().c_str(), USAGE);
  }
  if(argc < i + 2) {
    errmsg << "Missing argument for option '" << f << "'";
    throw PlowException("parseFilter", errmsg.str().c_str(), USAGE);
  }

  char pre[9];
  char post[7];

  if(argv[i][2] == 0) {
    sprintf(pre, " LIKE '%%");
    sprintf(post, "%%' OR ");
  } else if(argv[i][2] == 'e') {
    sprintf(pre, "='");
    sprintf(post, "' OR ");
  } else if(argv[i][2] != 0) {
    errmsg << "Wrong syntax near '";
    errmsg << argv[i][1] << argv[i][2] << "'";
    throw PlowException("parseFilter", errmsg.str().c_str(), USAGE);
  }

  if(argv[i][0] == '+') {
    gsSelect.append("\tAND NOT (");
  } else {
    gsSelect.append("\tAND (");
  }

  int pos = ++i;
  while(i < argc && (argv[i][0]!='-' && argv[i][0]!='+')){
    gsSelect.append(fieldNames[f]);
    gsSelect.append(pre);
    gsSelect.append(argv[i]);
    gsSelect.append(post);
    ++i;
  }

  if(pos == i) {
    errmsg << "Missing argument for option '" << f << "'";
    throw PlowException("parseFilter", errmsg.str().c_str(), USAGE);
  }

  gsSelect.replace(gsSelect.length()-4, 4, ")\n");

  return i-1;
} // parseFilter()



/**
 * parses command-line arguments
 *
 * @param argc number of arguments
 * @param argv arguments
 *
 * @throws PlowException on any error
 */
void parseArgs(int argc, char** argv)
{
  for(int i = 1; i < argc; i++) {
    switch(argv[i][0]) {
      case '-': // -?
        switch(argv[i][1]) {

          case 0: // -
            throw PlowException("parseArgs",
                      "Missing option after -",
                      USAGE);
          break;

          case '-': // --
            if(strcmp(argv[i], "--noplay") == 0) {
              gbPlay = false;
            } else if(strcmp(argv[i], "--add") == 0) {
              gbAddToPlaylist = true;
            } else if(strcmp(argv[i], "--help") == 0) {
              printusage();
              delete gIniParser;
              exit(0);
            } else if(strcmp(argv[i], "--version") == 0) {
              cout << PACKAGE << " " << VERSION << endl;
              delete gIniParser;
              exit(0);
            } else if (strcmp(argv[i], "--set") == 0) {
              i = parseSetOptions(argc, argv, i + 1);
            } else {
              throw PlowException("parseArgs",
                        "Wrong syntax near '--'",
                        USAGE);
            }
          break;

          case 'q': // -q
            if(argv[i][2] != 0) {
              throw PlowException("parseArgs",
                        "Wrong syntax near '-q'",
                        USAGE);
            } else {
              if(argv[i + 1] == 0) {
                throw PlowException("parseArgs",
                          "Missing sql statement for option '-q'",
                          USAGE);
              } else {
                printResult(argv[i + 1]);
                exit(0);
              }
            }
          break;

          case 'S': // -S
            gbShuffle = true;
          break;

          case 'Q': // -Q
            gbShowquery = true;
          break;

          case 'C': // -C
            init();
            copy2Portable();
            delete gIniParser;
            exit(0);
          break;

          case 'I': // -I
            if(i + 1 < argc) {
              init();
              add2Db(argv[i+1],
                     gsDatabase.c_str(),
                     gsMusicDirectory.c_str());
              delete gIniParser;
              exit(0);
            } else {
              throw PlowException("parseArgs",
                                  "Missing argument for option '-I'",
                                  USAGE);
            }

          break;

          case 'L': // -L
            if(argv[i + 1]) {
              char query[42 + 4 * strlen(argv[i + 1])];
              sprintf(query,
                      "SELECT \"id_%s\", \"%s\"\
                          FROM \"tbl_%s\" ORDER BY \"%s\";",
                      argv[i + 1],
                      argv[i + 1],
                      argv[i + 1],
                      argv[i + 1]);
              printResult(query);
              delete gIniParser;
              exit(0);
            } else {
              throw PlowException("parseArgs",
                                  "Missing argument for option '-L'",
                                  USAGE);
            }
          break;

          // select player
          case '0': case '1': case '2': case '3': case '4':
          case '5': case '6': case '7': case '8': case '9':
            gcPlayerNumber = argv[i][1];
          break;

          default: // -?
            i = parseFilter(argc, argv, i);
          break;
        } // switch(argv[i][1])
      break; // case '-'

      case '+': // +?
        i = parseFilter(argc, argv, i);
      break;

      // parse abbrevations set in inifile
      // and call this function again
      case '.':  // .?
        if(argv[i][1] != 0) {
          char buf[strlen(argv[i]) + 6];
          sprintf(buf, "[abbr]%s", &argv[i][1]);

          // just a dummy - parseArgs ignores first one
          string abbr = "ARGV[0]_IGNORED ";

          abbr.append(gIniParser->get(buf));

          StringParser abbrP(abbr.c_str());
          parseArgs(abbrP.getSize(), abbrP.getArgv());
        } else {
          throw PlowException("parseArgs",
                              "Missing abbrevation name",
                              USAGE);
        }
      break;

      default: // ?
        throw PlowException("parseArgs","Wrong syntax", USAGE);
      break;
    } // switch(argv[i][0])
  } // for(int i = 0; i < argc; i++)
} // parseArgs()



/**
 * hmm
 */
int main(int argc, char** argv)
{
    bool forkplayer = true;  // wether or not to fork player

    ostringstream errmsg;

    gsPlowHome  = getenv("HOME");
    gsPlowHome += "/.plow";

    gsDatabase = gsPlowHome + "/plow.sqlite";
    gsPlaylist = gsPlowHome + "/plow.m3u";

    string order(";");

  try {
    parseArgs(argc, argv);

    init();

    if(gbShuffle) {
      order = "ORDER BY\n\tRANDOM();";
    } else if(!gIniParser->get("[general]order").empty()) {
      order  = "ORDER BY\n\t";
      order += gIniParser->get("[general]order");
      order += ";";
    }

    char select[strlen(SELECT) + gsMusicDirectory.size() + 1];
    sprintf(select, SELECT, gsMusicDirectory.c_str());

    char buffer[strlen(select) + strlen(WHERE) +\
                strlen(gsSelect.c_str()) + strlen(order.c_str()) + 1];

    sprintf(buffer, "%s%s%s%s", select, WHERE,
            gsSelect.c_str(), order.c_str());

    if(!gsUpdate.empty()) {
      string in;
      string update("UPDATE\n\ttbl_music\nSET\n\t");
      update += &(gsUpdate.c_str()[2]);
      update += "\nWHERE\n\tid_music IN\n(\n  ";
      update += SELECT_ID;
      update += WHERE;
      update += gsSelect;
      update += ");";
      Sqlite3Result *rs = exe(buffer);
      cout << "> Do you really want to update " << rs->rows();
      cout << " rows? y/[n] ";
      cin >> in;
      if(in[0] == 'y') {
        delete rs;
        rs = exe(update.c_str());
      } else {
        cout << "> aborted ..." << endl;
      }
      delete rs;
      if(gbShowquery) {
        cout << update << endl;
      }
      delete gIniParser;
      exit(0);
    }


    if(gbShowquery) {
      cout << buffer << "\n" << endl;
    }

    createList(buffer);

    ostringstream player;
    if(gbPlay) {
      string inivalue = "[general]player";
      inivalue.append(1, gcPlayerNumber);
      player << gIniParser->get(inivalue.c_str());
      if(player.str().empty()) {
        errmsg << "No player with number " << gcPlayerNumber;
        errmsg << " set in inifile. Using player 0.";
        PlowException *e = new PlowException("main", errmsg.str().c_str());
        e->print();
        delete e;
        inivalue = "[general]player0";
        player << gIniParser->get(inivalue.c_str());
        gcPlayerNumber = 0;
      }
      inivalue = "[general]playernofork";
      if(gIniParser->get(inivalue.c_str()).find_first_of(gcPlayerNumber) !=
          string::npos) {
        forkplayer = false;
      }
      delete gIniParser; gIniParser = NULL;
      player << " \"" << gsPlaylist << "\"";
      StringParser sp(player.str().c_str());

      char **playerArgs = sp.getArgv();

      int child_pid = 0;

      if(forkplayer) {
        child_pid = fork();
      }

      switch(child_pid) {
        case 0: /* child */
          execvp(playerArgs[0], playerArgs);
          errmsg << "Can't execute player (" << playerArgs[0] << ")";
          throw PlowException("main", errmsg.str().c_str());
        case -1: /* fork error */
          throw PlowException("main", "Can't fork() process");
      }
    } else {
      delete gIniParser;
    }
  } catch(PlowException &e) {
    e.print();
    return 1;
  } catch (string &str) {
    cerr << "Exception: " << str << endl;
    cout << endl;
    return 1;
  } catch (...) {
    cerr << "Unhandled exception" << endl;
    cout << endl;
    return 1;
  }
  return 0;
} // main()



/**
 *
 */
CStrMap *vorbisFields()
{
  CStrMap *fields = new CStrMap;

  (*fields)["id"       ] = gIniParser->get("[vorbis]id"       ).c_str();
  (*fields)["title"    ] = gIniParser->get("[vorbis]title"    ).c_str();
  (*fields)["artist"   ] = gIniParser->get("[vorbis]artist"   ).c_str();
  (*fields)["album"    ] = gIniParser->get("[vorbis]album"    ).c_str();
  (*fields)["part"     ] = gIniParser->get("[vorbis]part"     ).c_str();
  (*fields)["parts"    ] = gIniParser->get("[vorbis]parts"    ).c_str();
  (*fields)["track"    ] = gIniParser->get("[vorbis]track"    ).c_str();
  (*fields)["tracks"   ] = gIniParser->get("[vorbis]tracks"   ).c_str();
  (*fields)["genre"    ] = gIniParser->get("[vorbis]genre"    ).c_str();
  (*fields)["rating"   ] = gIniParser->get("[vorbis]rating"   ).c_str();
  (*fields)["mood"     ] = gIniParser->get("[vorbis]mood"     ).c_str();
  (*fields)["situation"] = gIniParser->get("[vorbis]situation").c_str();
  (*fields)["tempo"    ] = gIniParser->get("[vorbis]tempo"    ).c_str();
  (*fields)["language" ] = gIniParser->get("[vorbis]language" ).c_str();
  (*fields)["date"     ] = gIniParser->get("[vorbis]date"     ).c_str();
  (*fields)["comment"  ] = gIniParser->get("[vorbis]comment"  ).c_str();

  return fields;
}



/**
 *
 */
CStrMap *id3Fields()
{
  CStrMap *fields = new CStrMap;

  (*fields)["id"       ] = gIniParser->get("[id3v2]id"       ).c_str();
  (*fields)["rating"   ] = gIniParser->get("[id3v2]rating"   ).c_str();
  (*fields)["mood"     ] = gIniParser->get("[id3v2]mood"     ).c_str();
  (*fields)["situation"] = gIniParser->get("[id3v2]situation").c_str();
  (*fields)["tempo"    ] = gIniParser->get("[id3v2]tempo"    ).c_str();
  (*fields)["language" ] = gIniParser->get("[id3v2]language" ).c_str();

  return fields;
}



/**
 * see above
 */
void add2Db(char *path, const char *dbPath, const char *musicPath)
{
  map<int, CStrMap *> fieldNames;

  fieldNames[TagReader::VORBIS] = vorbisFields();
  fieldNames[TagReader::ID3V2]  = id3Fields();

  Sqlite3Result *sr, *sr2;
  string query, query2, query3;

  vector<string> fields;

  fields.push_back("artist");
  fields.push_back("album");
  fields.push_back("genre");
  fields.push_back("rating");
  fields.push_back("mood");
  fields.push_back("situation");
  fields.push_back("tempo");
  fields.push_back("language");

  const char *format = "%q";
  char *result;

  TagReader *tag;
  PrioQ *fnames = new PrioQ;

  ostringstream errmsg;
  string unsupportedFiles;

  StringParser *sp = new StringParser(gIniParser->get(
                              "[general]extensions").c_str());
  char **extensions  = sp->getArgv();
  getFiles(*fnames, path, true, extensions);
  delete sp;

  cout << "> " << fnames->size() << " files found." << endl;
  if(fnames->size() == 0) {
    delete fnames;
    return;
  }
  query.reserve(fnames->size() * 1024);
  Sqlite3 sql(dbPath);


  // remove old stuff from tbl_tmp
  sr = sql.exe("DELETE FROM tbl_tmp; VACUUM;");
  delete sr;

  //
  // read tags from files and insert it into tbl_tmp
  //

  cout << "> read infos from files ... " << endl;

  query = "BEGIN TRANSACTION;";
  int removepos = strlen(musicPath);
  while(!fnames->empty()) {
    cout << "." << flush;
    tag = new TagReader(fnames->top(), fieldNames);

    if(tag->fileType() != TagReader::UNKNOWN) {

      query2 = fnames->top();
      query2 = query2.substr(removepos);

      query  += "INSERT INTO tbl_tmp (tmp_file_id, tmp_file, ";
      query  += "tmp_artist, tmp_title, tmp_album, tmp_part, ";
      query  += "tmp_parts, tmp_track, tmp_tracks, tmp_genre, ";
      query  += "tmp_rating, tmp_date, tmp_comment, tmp_length";
      query  += ") VALUES (";

      result = sqlite3_mprintf(
          "'%q','%q','%q','%q','%q','%q','%q','%q','%q','%q','%q',\
            '%q','%q','%q');",
          tag->id(), query2.c_str(), tag->artist(), tag->title(),
          tag->album(), tag->part(), tag->parts(), tag->track(),
          tag->tracks(), tag->genre(), tag->rating(), tag->date(),
          tag->comment(), tag->length());

      query += result;
      sqlite3_free(result);
    } else {
      unsupportedFiles.append("\t");
      unsupportedFiles.append(fnames->top());
      unsupportedFiles.append("\n");
    }

    if(tag->error()) {
      errmsg << "Error (" << tag->error() << ") at ";
      errmsg << fnames->top() << endl;
      PlowException *e = new PlowException("add2Db",
                                 errmsg.str().c_str());
      e->print();
      delete e;
      errmsg.str("");
    }
    delete tag;
    delete[] fnames->top();
    fnames->pop();
  }
  query += "COMMIT TRANSACTION;";

  sr = sql.exe(query.c_str());
  delete sr;
  delete fnames;
  cout << "\n ... done." << endl;

  //
  // add new artists, albums and genres
  //

  string fieldtmp, fieldid;

  for(uint i = 0; i < fields.size(); i++) {
    cout << "> insert new " << fields.at(i) << "s ..." << flush;

    query = "BEGIN TRANSACTION;";

    query3  = "SELECT DISTINCT tmp_";
    query3 += fields.at(i);
    query3 += " FROM tbl_tmp;";

    sr = sql.exe(query3.c_str());

    fieldtmp = "tmp_" + fields.at(i);
    fieldid  = "id_" + fields.at(i);

    if(sql.error() == SQLITE_OK) {
      for(int j = 0; j < sr->rows(); j++) {
        query3 = "SELECT id_";
        query3 += fields.at(i);
        query3 += " FROM tbl_";
        query3 += fields.at(i);
        query3 += " WHERE ";
        query3 += fields.at(i);
        query3 += "='";

        result = sqlite3_mprintf(format, sr->get(j, fieldtmp.c_str()));
        query3 += result;
        sqlite3_free(result);

        query3 += "';";

        sr2 = sql.exe(query3.c_str());

        if(sr2->rows() == 0) {
          delete sr2;
          query2  = "BEGIN TRANSACTION;INSERT INTO tbl_";
          query2 += fields.at(i);
          query2 += " (";
          query2 += fields.at(i);
          query2 += ")  VALUES ('";

          result = sqlite3_mprintf(format, sr->get(j,
                                   fieldtmp.c_str()));
          query2 += result;
          sqlite3_free(result);

          query2 += "');";
          query2 += query3;
          query2 += "COMMIT TRANSACTION;";

          sr2 = sql.exe(query2.c_str());
          cout << "\n\t" << sr->get(j, fieldtmp.c_str()) << flush;
        }
        query += "UPDATE tbl_tmp SET tmp_id_";
        query += fields.at(i);
        query += "='";
        query += sr2->get(0, fieldid.c_str());
        query += "' WHERE tmp_";
        query += fields.at(i);
        query += "='";

        result = sqlite3_mprintf(format, sr->get(j, fieldtmp.c_str()));
        query += result;
        sqlite3_free(result);

        query += "';";

        delete sr2;
      }

      query += "COMMIT TRANSACTION;";

      sr2 = sql.exe(query.c_str());
      delete sr2;

    }
    delete sr;
    cout << "\n ... done." << endl;
  }


  //
  // set additional info to albums
  //

  // set album_id_artist for album if it isn't a sampler
  // (more than 3 songs from one artist)
  cout << "> add info to (new) albums ..." << endl;

  query  = "SELECT tmp_id_album, tmp_id_artist FROM ";
  query += "tbl_tmp GROUP BY tmp_id_artist, tmp_id_album ";
  query += "HAVING COUNT(*) > 3;";

  sr = sql.exe(query.c_str());

  query = "BEGIN TRANSACTION;";

  if(sql.error() == SQLITE_OK) {
    for(int i = 0; i < sr->rows(); i++) {
      query += "UPDATE tbl_album SET album_id_artist='";
      query += sr->get(i, "tmp_id_artist");
      query += "' WHERE id_album='";
      query += sr->get(i, "tmp_id_album");
      query += "';";

    }
    query += " COMMIT TRANSACTION;";
    sr2 = sql.exe(query.c_str());
    delete sr2;
  }
  delete sr;

  // add number of parts and tracks to tbl_album
  sr = sql.exe(
      "SELECT DISTINCT tmp_id_album, tmp_parts, tmp_tracks FROM tbl_tmp;");

  query = "BEGIN TRANSACTION;";
  for(int i = 0; i < sr->rows(); i++) {
    query += "UPDATE tbl_album SET parts='";
    query += sr->get(i, "tmp_parts");
    query += "', tracks='";
    query += sr->get(i, "tmp_tracks");
    query += "' WHERE id_album='";
    query += sr->get(i, "tmp_id_album");
    query += "';";
  }
  delete sr; sr = NULL;
  query += "COMMIT TRANSACTION;";
  sr = sql.exe(query.c_str());
  delete sr; sr = NULL;
  cout << " ... done." << endl;


  //
  // insert new songs
  //

  sr = sql.exe(
    "SELECT * FROM tbl_tmp ORDER BY tmp_album, tmp_part, tmp_track;");

  int count = 0;

  query = "BEGIN TRANSACTION;";

  for(int i = 0; i < sr->rows(); i++) {
    query3  = "SELECT file FROM tbl_music WHERE file='";

    result = sqlite3_mprintf(format, sr->get(i, "tmp_file"));
    query3 += result;
    sqlite3_free(result);

    query3 += "';";

    sr2 = sql.exe(query3.c_str());

    if(sr2->rows() == 0) {
      ++count;

      query += "INSERT INTO tbl_music (file_id, file, title, ";
      query += " _id_artist, _id_album, part, track, _id_genre,";
      query += " _id_rating, _id_mood, _id_situation, _id_tempo,";
      query += "_id_language, date, comment, length) VALUES (";

      result = sqlite3_mprintf(
          "'%q','%q','%q','%q','%q','%q','%q','%q','%q','%q',\
              '%q','%q','%q','%q','%q','%q');",
          sr->get(i,"tmp_file_id"),      sr->get(i,"tmp_file"),
          sr->get(i,"tmp_title"),        sr->get(i,"tmp_id_artist"),
          sr->get(i,"tmp_id_album"),     sr->get(i,"tmp_part"),
          sr->get(i,"tmp_track"),        sr->get(i,"tmp_id_genre"),
          sr->get(i,"tmp_id_rating"),    sr->get(i,"tmp_id_mood"),
          sr->get(i,"tmp_id_situation"), sr->get(i,"tmp_id_tempo"),
          sr->get(i,"tmp_id_language"),  sr->get(i,"tmp_date"),
          sr->get(i,"tmp_comment"),      sr->get(i,"tmp_length"));

      query += result;
      sqlite3_free(result);
    }
    delete sr2;
  }

  query += "COMMIT TRANSACTION";

  sr2 = sql.exe(query.c_str());
  delete sr2;
  delete sr;

  sr = sql.exe("DELETE FROM tbl_tmp; VACUUM;");
  delete sr;
  cout << "> " << count << " new files inserted." << endl;

  delete fieldNames[TagReader::VORBIS];
  delete fieldNames[TagReader::ID3V2];

  if(unsupportedFiles.length()) {
    cout << "> The following files don't contain a supported tag:" << endl;
    cout << unsupportedFiles.c_str() << flush;
  }
} // add2Db()
