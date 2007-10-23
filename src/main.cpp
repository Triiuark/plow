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
#ifndef PACKAGE_NAME
#define PACKAGE_NAME "plow"
#endif


#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

#include <cstring>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/statvfs.h>

#include <errno.h>


#include "helper.h"
#include "PlowException.h"
#include "IniParser.h"
#include "Sqlite3.h"
#include "Sqlite3Result.h"

#include "StringParser.h"

#include <string>
#include <vector>
#include <queue>

#include <sqlite3.h> // for error constants

#include "global.h"
#include "TagReader.h"

using namespace std;

const char *USAGE = "Usage: plow [LIST_OPTIONS]|[-L <tbl>]|[-q <sql>]\
|[-C]|[-I <dir>]|[--help]";

const char *SELECT  = "SELECT\n\t'%s' || file as file, artist, title, length,\
 album, genre,\n\tlanguage, mood, tempo, rating, situation, part,\
 track,\n\tparts, tracks\nFROM\n\ttbl_music, tbl_artist, tbl_album,\
 tbl_genre, tbl_rating,\n\ttbl_language,tbl_mood, tbl_situation,\
 tbl_tempo\n";

const char *SELECT_ALL  = "SELECT\n\t*\
 \nFROM\n\ttbl_music, tbl_artist, tbl_album,\
 tbl_genre, tbl_rating,\n\ttbl_language,tbl_mood, tbl_situation,\
 tbl_tempo\n";

const char *WHERE   = "WHERE\n\tid_artist=_id_artist AND\
 id_album=_id_album\n\tAND id_genre=_id_genre AND\
 id_rating=_id_rating\n\tAND id_language=_id_language AND\
 id_mood=_id_mood\n\tAND id_situation=_id_situation AND\
 id_tempo=_id_tempo\n";

IniParser *gipIni;

string gsSelect;

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



void add2Db(char *path, const char *dbPath, const char *musicPath);

void printusage() {
  cout << USAGE << endl;
  cout << endl;
  cout << "                 playlist options"                      << endl;
  cout << "                ****************************************";
  cout << "**************"                                         << endl;
  cout << "-|+<C>[e] str   - search for fields containing str"     << endl;
  cout << "                + search for fields not containing str" << endl;
  cout << "                e str matches exactly"                  << endl;
  cout << "                <C> is one of A (album),     a (artist),";
  cout << " g (genre),"                                            << endl;
  cout << "                              l (language),  m (mood),  ";
  cout << " r (rating),"                                           << endl;
  cout << "                              s (situation), t (tempo), ";
  cout << " T (title)"                                             << endl;
  cout << "-0...9          select player number set in configuration";
  cout << " file"                                                  << endl;
  cout << "-Q              print out the sql statement"            << endl;
  cout << "-S              shuffle the playlist"                   << endl;
  cout << "--add           append to playlist"                     << endl;
  cout << "--noplay        don't start a player"                   << endl;
  cout << endl;
  cout << endl;
  cout << "                 print table"                           << endl;
  cout << "                ****************************************";
  cout << "**************"                                         << endl;
  cout << "-L <tbl>        <tbl> is one of album, artist,";
  cout << " genre,     language,"                                  << endl;
  cout << "                                mood,  rating,";
  cout << " situation, tempo"                                      << endl;
  cout << endl;
  cout << endl;
  cout << "                 execute sql statement"                 << endl;
  cout << "                ****************************************";
  cout << "**************"                                         << endl;
  cout << "-q <sql>        executes statement <sql>"               << endl;
  cout << endl;
  cout << endl;
  cout << "                 copy playlist"                         << endl;
  cout << "                ****************************************";
  cout << "**************"                                         << endl;
  cout << "-C              copy all files in the current playlist" << endl;
  cout << "                to portable_device set in config file"  << endl;
  cout << endl;
  cout << endl;
  cout << "                 insert new files into db"              << endl;
  cout << "                ****************************************";
  cout << "**************"                                         << endl;
  cout << "-I <dir>        insert all (supported) files in <dir>";
  cout << " into database"                                         << endl;
  cout << "                Note: <dir> has to be an absolute path" << endl;
  cout << endl;
  cout << endl;
  cout << "                 help"                                  << endl;
  cout << "                ****************************************";
  cout << "**************"                                         << endl;
  cout << "--help          print this message"                     << endl;
  cout << endl;
  cout << endl;
  cout << "                 examples"                              << endl;
  cout << "                ****************************************";
  cout << "**************"                                         << endl;
  cout << "plow -a queen               ";
  cout << " # artist contains queen"                               << endl;
  cout << "plow -a queen -T rock       ";
  cout << " # artist contains queen"                               << endl;
  cout << "                             # AND title contains rock" << endl;
  cout << "plow -ae Queen              ";
  cout << " # artist is exactly Queen"                             << endl;
  cout << "plow +a queen               ";
  cout << " # artist NOT contains queen"                           << endl;
  cout << "plow +ae Queen              ";
  cout << " # artist IS NOT Queen"                                 << endl;
  cout << "plow -a queen beatles       ";
  cout << " # artist contains queen OR beatles"                    << endl;
  cout << "plow -a queen +a queens     ";
  cout << " # artist contains queen AND NOT queens"                << endl;
  cout << "plow -a queen -a \"stone age\"";
  cout << " # artist contains queen AND stone age"                 << endl;
} // printusage()

void errorhandler(const char *errstr) {
  cerr << "plow: " << errstr << endl;
  cout << endl;
  cout << USAGE << endl;
  exit(1);
} // errorhandler

Sqlite3Result *exe(const char *query, int *err) {
  Sqlite3 sql(gsDatabase.c_str());
  Sqlite3Result *rs = sql.exe(query);
  *err = 0;
  if(sql.error()) {
    cout << "SQL error: ";
    cout << sql.errmsg() << endl;
    cout << query;
    cout << endl;
    delete rs; rs = NULL;
    *err = 1;
  }
  return rs;
} // exe()

/********************************************************************
 *  functions for copy list to portable                             *
 ********************************************************************/

/*
 * get the required fields for the filename
 */
string infoString(Sqlite3Result &rs,
                  uint row,
                  const vector<string *> &tokens,
                  bool removeslash = false) {

  const string  forbidden = "\\\n\r\" '$@*{}[]()/:;&?";

  string        out;
  string        tmp;
  string        tmp2;
  ostringstream tmp3;

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
    if(tokens[i]->c_str()[0] == '%'
        && tokens[i]->c_str()[strsize - 1] == '%') {
      if(*tokens[i] == "%track0%") {
        if(strlen(rs.get(row, "track")) < 2) {
          tmp += "0";
        }
        tmp += rs.get(row, "track");
      } else if(*tokens[i] == "%tracks0%") {
        if(strlen(rs.get(row, "tracks")) < 2) {
          tmp += "0";
        }
        tmp += rs.get(row, "tracks");
      } else if(*tokens[i] == "%part0%") {
        if(strlen(rs.get(row, "part")) < 2) {
          tmp += "0";
        }
        tmp += rs.get(row, "part");
      } else if(*tokens[i] == "%parts0%") {
        if(strlen(rs.get(row, "parts")) < 2) {
          tmp += "0";
        }
        tmp += rs.get(row, "parts");
      } else if(*tokens[i] == "%fileext%") {
        tmp2 = rs.get(row, "file");
        uint pos = tmp2.rfind('.');
        if(pos != string::npos) {
          tmp += tmp2.substr(pos);
        }
      } else if(*tokens[i] == "%lengths%") {
        tmp3.str("");
        tmp3 << (uint)(atof(rs.get(row, "length"))/1000.0);
        tmp += tmp3.str();
      } else if(*tokens[i] == "%artistOrAlbum%") {
        if(strcmp(rs.get(row, "album_id_artist"), "1") != 0) {
          tmp += rs.get(row, "artist");
        } else {
          tmp += rs.get(row, "album");
        }
      } else if(*tokens[i] == "%albumOrArtist%") {
        if(strcmp(rs.get(row, "album_id_artist"), "1") != 0) {
          tmp += rs.get(row, "album");
        } else {
          tmp += rs.get(row, "artist");
        }
      } else if(*tokens[i] == "%albumOrEmpty%") {
        if(strcmp(rs.get(row, "album_id_artist"), "1") != 0) {
          tmp += rs.get(row, "album");
        } else {
          lastEmpty = true;
        }
      } else if(*tokens[i] == "%emptyOrAlbum%") {
        if(strcmp(rs.get(row, "album_id_artist"), "1") == 0) {
          tmp += rs.get(row, "album");
        } else {
          lastEmpty = true;
        }
      } else if(*tokens[i] == "%artistOrEmpty%") {
        if(strcmp(rs.get(row, "album_id_artist"), "1") != 0) {
          tmp += rs.get(row, "artist");
        } else {
          lastEmpty = true;
        }
      } else if(*tokens[i] == "%emptyOrArtist%") {
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

/*
 * copy files
 */
int copy2Portable() {
  /* TODO beware of different blocksizes!!! */
  struct statvfs stfs;
  struct stat    st;
  int err = 0;

  string portable = gipIni->get("[general]portable");
  cout << "copy playlist to " << portable;
  vector<string> *files = new vector<string>;

  err = statvfs(portable.c_str() , &stfs);
  if(err) {
    delete files; files = NULL;
    throw PlowException("copy2Portable", portable.c_str());
  }

  uint64_t freebytes = (stfs.f_bsize / 1024) * stfs.f_bfree;
  cout << " (" << freebytes << " bytes free) ..." << endl;

  ifstream inputStream;
  inputStream.open(gsPlaylist.c_str(), ios::in);
  if(!inputStream) {
    delete files; files = NULL;
    throw PlowException("copy2Portable", gsPlaylist.c_str());
  }
  char *buf = new char[1024];
  string *file;
  unsigned long size = 0;
  int bla = 0;
  while(inputStream.getline(buf, 1024)) {
    if(buf[0] != '#') {
      bla++;
      err = stat(buf, &st);
      if(err) {
        cout << strerror(errno) << endl;
      } else {
        size += st.st_size / 1024;
        file = new string(buf);
        //sprintf(file, buf);
        files->push_back(*file);
        //cout << buf << endl;
      }
    }
  }
  delete[] buf;

  char cs = 's';
  if(bla == 1)
  {
    cs = '\0';
  }
  cout << "... " << bla << " file" << cs << " (" << size;
  cout << " bytes) ..." << endl;
  long result = freebytes - size;
  if(result < 0) {
    cout << "not enough space available." << endl;
    delete files; files = NULL;
    return -1;
  }

  string dst;
  string f;
  string *s;
  string query(SELECT_ALL);
  query.append(WHERE);
  string q;
  Sqlite3Result *rs;
  StringParser sp(gipIni->get("[general]portable_name").c_str());
  vector<string *> namestring = sp.getTokens();
  for(uint i = 0, j = files->size(); i < files->size(); i++, j--) {
    s = new string(files->at(i));
    f = (s->substr(gsMusicDirectory.size()));

    if(!gipIni->get("[general]portable_name").empty()) {
      q = query;
      q.append(" AND file='");
      q.append(f);
      q.append("';");
      rs = exe(q.c_str(), &err);
      if(err) {
        cerr << "Error " << endl;
        continue;
      }
      f = infoString(*rs, 0, namestring, true);
    }

    dst = portable + "/" + f;

    delete s; s = NULL;
    copyfile(files->at(i), dst);
    cout << j<< " " << flush;
  }
  delete files; files = NULL;

  cout <<  "done" << endl;

  inputStream.close();

  return 0;
} //copy2Portable

/********************************************************************
 * functions for -q and -L options                                  *
 ********************************************************************/

/*
 *  executes query and prints out the result as a nice table
 */
void printResult(const char *query) {
  int err;
  Sqlite3Result *rs = exe(query, &err);
  if(err) {
    exit(1);
  }
  if(rs->cols()) {
    //rs->maxlen();
    cout << "| ";
    int len = 0;
    for(int i = 0; i < rs->cols(); i++) {
      len = strlen(rs->getHead(i)) - utf8strlen(rs->getHead(i));
      cout << setw(rs->getWidth(i) + len);
      cout <<  setiosflags(ios::left) << rs->getHead(i);
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
  delete rs; rs = NULL;
  exit(0);
} // printResult()

/********************************************************************
 * functions for creating playlist                                  *
 ********************************************************************/
int createList(const char *query) {
  int err;
  Sqlite3Result *rs = exe(query, &err);
  if(err) {
    return 1;
  }
  if(rs->rows() == 0) {
    gbPlay = false;
    cout << "Empty result. Sorry." << endl;
    delete rs; rs = NULL;
    return 0;
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
  err = errno;
  if(!m3u) {
    cout << "\nError opening playlistfile (" << gsPlaylist << "): ";
    cout << strerror(err) << endl;
    delete rs; rs = NULL;
    return 1;
  }
  if(!gbAddToPlaylist) {
    m3u << "#EXTM3U" << endl;
  }
  uint playtime = 0, total_playtime = 0;
  StringParser sp(gipIni->get("[general]extinf").c_str());
  bool extinf = true;
  if(gipIni->get("[general]extinf").empty()) {
    extinf = false;
  }
  //exit(0);
  for(int i = 0; i < rs->rows(); i++) {
    string info = infoString(*rs, i, sp.getTokens());
    playtime = (uint)(atof(rs->get(i, "length")) / 1000.0);
    total_playtime += playtime;
    if(extinf) {
      m3u << "#EXTINF:" << info << endl;
    }
    m3u << rs->get(i, 0);
    m3u << endl;
  }
  m3u.close();
  uint hours = (total_playtime / 3600);
  uint mins  = (total_playtime - (hours * 3600)) / 60;
  uint secs  = (total_playtime - (hours * 3600)) - (mins * 60);
  cout << " (" << hours << "h" << mins << "m" << secs << "s)." << endl;
  delete rs; rs = NULL;
  return 0;
} // createList()

int parseField(int argc, char **argv, int i) {

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

  if(fieldNames[f] == "") {
    errorhandler("unknown option");
  }
  if(argc < i + 2) {
    errorhandler("missing argument");
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
    errorhandler("wrong synatx");
  }

  if(argv[i][0] == '+') {
    gsSelect.append("\tAND NOT (");
  } else {
    gsSelect.append("\tAND (");
  }


  i++;
  while(i < argc && (argv[i][0]!='-' && argv[i][0]!='+')){
    gsSelect.append(fieldNames[f]);
    gsSelect.append(pre);
    gsSelect.append(argv[i]);
    gsSelect.append(post);
    i++;
  }
  gsSelect.replace(gsSelect.length()-4, 4, ")\n");

  return i-1;
} // parseField()

void parseArgs(int argc, char** argv) {
  for(int i = 1; i < argc; i++) {
    //cout << i << ": " << argv[i] << endl;
    switch(argv[i][0]) {
      case '-': // -?
        switch(argv[i][1]) {

          case 0: // -
            errorhandler("missing argument");
          break;

          case '-': // --
            if(strcmp(argv[i], "--noplay") == 0) {
              gbPlay = false;
            } else if (strcmp(argv[i], "--add") == 0) {
              gbAddToPlaylist = true;
            } else if (strcmp(argv[i], "--help") == 0) {
              printusage();
              exit(0);
            } else {
              errorhandler("wrong syntax near --");
            }
          break;

          case 'q': // -q
            if(argv[i][2] != 0) {
              errorhandler("wrong syntax near -q");
            } else {
              if(argv[i + 1] == 0) {
                errorhandler("Missing sql statement for option -q");
              } else {
                printResult(argv[i + 1]);
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
            copy2Portable();
            exit(0);
          break;

          case 'I': // -I
            if(i + 1 < argc) {
              add2Db(argv[i+1],
                     gsDatabase.c_str(),
                     gsMusicDirectory.c_str());
              exit(0);
            } else {
              errorhandler("missing argument for option I");
            }

          break;

          case 'L': // -L
            if(argv[i + 1]) {
              char query[42 + 4 * strlen(argv[i + 1])];
              sprintf(query,
                  "SELECT \"id_%s\", \"%s\" FROM \"tbl_%s\" ORDER BY \"%s\";",
                  argv[i + 1], argv[i + 1], argv[i + 1], argv[i + 1]);
              printResult(query);
            } else {
              errorhandler("missing argument for option -L");
            }
          break;

          /* select player */
          case '0': case '1': case '2': case '3': case '4':
          case '5': case '6': case '7': case '8': case '9':
            gcPlayerNumber = argv[i][1];
          break;

          default: // -?
            i = parseField(argc, argv, i);
          break;
        } // switch(argv[i][1])
      break; // case '-'

      case '+': // +?
        i = parseField(argc, argv, i);
      break;

      /* parse abbrevations set in inifile and call this function again */
      case '.':  // .?
        if(argv[i][1] != 0) {
          char buf[strlen(argv[i]) + 6];
          sprintf(buf, "[abbr]%s", &argv[i][1]);

          /* just a dummy - parseArgs ignores first one */
          string abbr = "ARGV[0]_IGNORED ";

          abbr.append(gipIni->get(buf));

          StringParser abbrP(abbr.c_str());
          parseArgs(abbrP.getSize(), abbrP.getArgv());
        } else {
          errorhandler("missing abbrevation name");
        }
      break;

      default: // ?
        errorhandler("wrong syntax");
      break;
    } // switch(argv[i][0])
  } // for(int i = 0; i < argc; i++)
} // parseArgs()



int main(int argc, char** argv) {
  try {

    bool forkplayer     = true;  // wether or not to fork player

    ostringstream errmsg;

    string pdir = getenv("HOME");
    pdir.append("/.plow");

    gsDatabase   = pdir + "/plow.db";
    gsPlaylist = pdir + "/plow.m3u";

    string inifile = pdir + "/plowrc";

    string order = ";";
    gipIni = new IniParser(inifile.c_str());
    if(!gipIni->error()) {
      gsMusicDirectory = gipIni->get("[general]path");

      if(!gipIni->get("[general]playlist").empty()) {
        gsPlaylist = gipIni->get("[general]playlist");
      }
    } else {
      errmsg << "Error ";
      cout << "inifile error: " << gipIni->error() << endl;
      cout << inifile << endl;
    }

    parseArgs(argc, argv);

    /* if we're still here we have to build a playlist ;-) */

    if(!gbShuffle && !gipIni->get("[general]order").empty()) {
      order  = "ORDER BY\n\t";
      order += gipIni->get("[general]order");
      order += ";";
    } else if(gbShuffle) {
      order = "ORDER BY\n\tRANDOM();";
    }

    char select[strlen(SELECT) + gsMusicDirectory.size() + 1];
    sprintf(select, SELECT, gsMusicDirectory.c_str());

    char buffer[strlen(select) + strlen(WHERE) +\
                strlen(gsSelect.c_str()) + strlen(order.c_str()) + 1];

    sprintf(buffer, "%s%s%s%s", select, WHERE,
            gsSelect.c_str(), order.c_str());


    if(gbShowquery) {
      cout << buffer << "\n" << endl;
    }

    int err = createList(buffer);

    if(err) {return 1;}

    ostringstream player;
    if(gbPlay) {
      string inivalue = "[general]player";
      inivalue.append(1, gcPlayerNumber);
      player << gipIni->get(inivalue.c_str());
      if(player.str().empty()) {
        cerr << "No player with number " << gcPlayerNumber << flush;
        cerr << " set in inifile. Using player 0." << endl;
        inivalue = "[general]player0";
        player << gipIni->get(inivalue.c_str());
        gcPlayerNumber = 0;
      }
      inivalue = "[general]playernofork";
      if(gipIni->get(inivalue.c_str()).find_first_of(gcPlayerNumber) !=
          string::npos) {
        forkplayer = false;
      }
      delete gipIni; gipIni = NULL;
      player << " \"" << gsPlaylist << "\"";
      StringParser sp(player.str().c_str());

      char **playerArgs = sp.getArgv();

      int child_pid = 0;

      if(forkplayer) {
        child_pid = fork();
        err = errno;
      }

      switch(child_pid) {
        case 0: /* child */
          execvp(playerArgs[0], playerArgs);
          err = errno;
          errmsg << "Can't execute player (" << playerArgs[0] << "): ";
          errmsg << strerror(err);
          errorhandler(errmsg.str().c_str());
          //break;
        case -1: /* fork error */
          errmsg << "Can't fork() process: " << strerror(err);
          errorhandler(errmsg.str().c_str());
          //break;
      }
    } else {
      delete gipIni;
    }
  } catch(PlowException &e) {
    e.print();
    return 1;
  } catch (string &str) {
    cerr << "Exception: " << str << endl;
    return 1;
  } catch (...) {
    cerr << "Unhandled exception" << endl;
    return 1;
  }
  return 0;
} // main()


void add2Db(char *path, const char *dbPath, const char *musicPath) {

  Sqlite3Result *sr, *sr2;
  string query, query2, query3;

  vector<string> fields;

  fields.push_back("artist");
  fields.push_back("album");
  fields.push_back("genre");
  fields.push_back("rating");

  const char *format = "%q";
  char *result;

  TagReader *tag;
  p_queue *fnames = new p_queue;

  getFiles(fnames, path, true);
  cout << ">" << fnames->size() << " files found." << endl;
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

  cout << ">read infos from files ... " << endl;

  query = "BEGIN TRANSACTION;";
  int removepos = strlen(musicPath);
  while(!fnames->empty()) {
    cout << "." << flush;
    tag = new TagReader(fnames->top());

    if(tag->fileType() != TagReader::UNKNOWN) {

      query2 = fnames->top();
      query2 = query2.substr(removepos);

      query  += "INSERT INTO tbl_tmp (tmp_file_id, tmp_file, ";
      query  += "tmp_artist, tmp_title, tmp_album, tmp_part, ";
      query  += "tmp_parts, tmp_track, tmp_tracks, tmp_genre, ";
      query  += "tmp_rating, tmp_release, tmp_comments, tmp_length";
      query  += ") VALUES (";

      result = sqlite3_mprintf(
          "'%q','%q','%q','%q','%q','%q','%q','%q','%q','%q','%q',\
            '%q','%q','%q');",
          tag->id(), query2.c_str(), tag->artist(), tag->title(),
          tag->album(), tag->part(), tag->parts(), tag->track(),
          tag->tracks(), tag->genre(), tag->rating(), tag->year(),
          tag->comment(), tag->length());

      query += result;
      sqlite3_free(result);
    }

    if(tag->error()) {
      cerr << "Error (" << tag->error() << ") at ";
      cerr << fnames->top() << endl;
    }
    delete tag;
    delete[] fnames->top();
    fnames->pop();
  }
  query += "COMMIT TRANSACTION;";

  sr = sql.exe(query.c_str());
  if(sql.error()) {
    cerr << sql.errmsg() << endl;
  }
  delete sr;

  delete fnames; fnames = NULL;
  cout << "\n ... done." << endl;

  //
  // add new artists, albums and genres
  //

  string fieldtmp, fieldid;

  for(uint i = 0; i < fields.size(); i++) {
    cout << ">insert new " << fields.at(i) << "s ..." << flush;

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
      if(sql.error()) cout << sql.errmsg()<<endl;
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
  cout << ">add info to (new) albums ..." << endl;

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
    if(sql.error()) {
      cerr << sql.errmsg() << endl;
    }
    delete sr2;
  }
  delete sr;

  // add number of parts and tracks to tbl_album
  sr = sql.exe(
      "SELECT DISTINCT tmp_id_album, tmp_parts, tmp_tracks FROM tbl_tmp;");

  query = "BEGIN TRANSACTION;";
  if(sql.error()) cout << sql.errmsg() << endl;
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
  if(sql.error()) {
    cerr << sql.errmsg() << endl;
  }
  delete sr; sr = NULL;
  cout << " ... done." << endl;


  //
  // insert new songs
  //

  sr = sql.exe(
    "SELECT * FROM tbl_tmp ORDER BY tmp_album, tmp_part, tmp_track;");

  int count = 0;

  if(sql.error() == 0) {

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

        query += "INSERT INTO tbl_music (file_id, file, _id_artist,";
        query += " title, _id_album, part, track, _id_genre,";
        query += " _id_rating, release, comments, length) VALUES (";

        result = sqlite3_mprintf(
            "'%q','%q','%q','%q','%q','%q','%q','%q','%q','%q',\
                '%q','%q');",
            sr->get(i,"tmp_file_id"), sr->get(i,"tmp_file"),
            sr->get(i,"tmp_id_artist"), sr->get(i,"tmp_title"),
            sr->get(i,"tmp_id_album"), sr->get(i,"tmp_part"),
            sr->get(i,"tmp_track"), sr->get(i,"tmp_id_genre"),
            sr->get(i,"tmp_id_rating"), sr->get(i,"tmp_release"),
            sr->get(i,"tmp_comments"), sr->get(i,"tmp_length"));

        query += result;
        sqlite3_free(result);
      }
      delete sr2;
    }

    query += "COMMIT TRANSACTION";

    sr2 = sql.exe(query.c_str());
    if(sql.error() != 0) {
      cerr << sql.errmsg() << endl;
    }
    delete sr2;
  } else {
    cerr << sql.errmsg() << endl;
  }
  delete sr;

  sr = sql.exe("DELETE FROM tbl_tmp; VACUUM;");
  delete sr;
  cout << ">" << count << " new files inserted." << endl;
} // add2Db()
