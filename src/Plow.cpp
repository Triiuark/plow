#include "Plow.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#include <cerrno>

extern "C" {
#include <sys/stat.h>
}

#include "constants.h"
#include "helper.h"
#include "StringParser.h"

#include "ReaderSelector.h"
#include "AbstractReader.h"


using namespace std;

map<char, pair<const char *,Plow::FieldType> > Plow::fields;

void Plow::initFields()
{
  if(Plow::fields.size() == 0)
  {
    fields['A'] = pair<const char *, FieldType> ("album",         TABLE);
    fields['a'] = pair<const char *, FieldType> ("artist",        TABLE);
    fields['c'] = pair<const char *, FieldType> ("comment",       TEXT);
    fields['d'] = pair<const char *, FieldType> ("date",          TEXT);
    fields['f'] = pair<const char *, FieldType> ("file",          TEXT);
    fields['g'] = pair<const char *, FieldType> ("genre",         TABLE);
    fields['i'] = pair<const char *, FieldType> ("file_id",       TEXT);
    fields['l'] = pair<const char *, FieldType> ("language",      TABLE);
    fields['L'] = pair<const char *, FieldType> ("lyrics",        TEXT);
    fields['m'] = pair<const char *, FieldType> ("mood",          TABLE);
    fields['N'] = pair<const char *, FieldType> ("tracks",        NUMBER);
    fields['n'] = pair<const char *, FieldType> ("track",         NUMBER);
    fields['P'] = pair<const char *, FieldType> ("parts",         NUMBER);
    fields['p'] = pair<const char *, FieldType> ("part",          NUMBER);
    fields['R'] = pair<const char *, FieldType> ("album_release", TEXT);
    fields['r'] = pair<const char *, FieldType> ("rating",        TABLE);
    fields['S'] = pair<const char *, FieldType> ("status",        NUMBER);
    fields['s'] = pair<const char *, FieldType> ("situation",     TABLE);
    fields['T'] = pair<const char *, FieldType> ("title",         TEXT);
    fields['t'] = pair<const char *, FieldType> ("tempo",         TABLE);
    fields['x'] = pair<const char *, FieldType> ("length",        NUMBER);
  }
}



pair<const char *, Plow::FieldType> *Plow::getField(char key)
{
  Plow::initFields();

  return &fields[key];
}




Plow::Plow()
{
  mKeepBackup      = 5;
  mPlayer        = '0';
  mShowQuery     = false;
  mPlay          = true;
  mAddToPlaylist = false;
  mShuffle       = false;
  mCopy          = false;
}



void Plow::setPlay(bool play)
{
  mPlay = play;
}



void Plow::setCopy(bool copy)
{
  mCopy = copy;
}



void Plow::setRandom(bool random)
{
  mShuffle = random;
}



void Plow::setShowQuery(bool show)
{
  mShowQuery = show;
}



void Plow::setAppend(bool append)
{
  mAddToPlaylist = append;
}



void Plow::setPlayer(char player)
{
  mPlayer = player;
}



void Plow::setQuery(const char * const query)
{
  mQuery = query;
}



void Plow::printQuery(string const &query)
{
  if(mShowQuery)
  {
    cout << query << endl;
  }
}



void Plow::createFileNames()
{
  mConfigDir    = getenv("HOME");
  mConfigDir   += "/.plow";
  mDataDir      = mConfigDir;
  mDbFile       = mDataDir + "/plow.sqlite";
  mPlaylistFile = mDataDir + "/plow.m3u";
}



void Plow::initialize()
{
  if(mIniParser.get() == 0)
  {
    createFileNames();

    struct stat st;

    if(stat(mDbFile.c_str(), &st) == -1)
    {
      mkdir_r(mDataDir);

      if(errno == ENOENT)
      {
        openDb();
        mSqlite3->exe(DATABASE);

        cout << "> created database: " << mDbFile << endl;
      }
    }

    string inifile = mConfigDir + "/plow.conf";

    if(stat(inifile.c_str(), &st) == -1)
    {
      mkdir_r(mConfigDir);

      if(errno == ENOENT)
      {
        ofstream fout(inifile.c_str());
        if(!fout)
        {
          throw PlowException("Plow::initialize",
                              "can't create config file");
        }
        fout << INI_FILE;
        fout.close();

        cout << "> created config file: " << inifile << "\n\n"
             << "You have to edit " << inifile << " now." << endl;

        exit(0);
      }
    }

    mIniParser.reset(new IniParser(inifile.c_str()));

    mMusicDir = mIniParser->get("general/path");

    if(!mIniParser->get("general/playlist").empty())
    {
      mPlaylistFile = mIniParser->get("general/playlist");
    }

    if(!mIniParser->get("general/backups").empty())
    {
      mKeepBackup = atoi(mIniParser->get("general/backups").c_str());
    }
  }
}



void Plow::openDb()
{
  if(mSqlite3.get() == 0)
  {
    mSqlite3.reset(new Sqlite3(mDbFile.c_str()));
  }
}



void Plow::backup()
{
  if(mKeepBackup < 1)
  {
    return;
  }

  char timeBuff[24];
  struct tm *tmp;
  time_t secSince = time(NULL);

  tmp = localtime(&secSince);
  strftime(timeBuff, 23, "%Y%m%d%H%M%S", tmp);

  string dst = mDataDir + "/.";
  dst += timeBuff;
  dst += ".sqlite~";

  copyfile(mDbFile, dst);


  PrioQ fnames;
  char  *path         = new char[mDataDir.size() + 1];
  char  *extension[8] = {"sqlite~"};

  sprintf(path, "%s", mDataDir.c_str());

  getFiles(fnames, path, false, extension);

  int toRemove = fnames.size() - mKeepBackup;

  for(int i = 0; i < toRemove; ++i)
  {
    unlink(fnames.top());
    delete[] fnames.top();
    fnames.pop();
  }

  while(!fnames.empty())
  {
    delete[] fnames.top();
    fnames.pop();
  }

  delete[] path;
}



void Plow::appendUpdate(char key, const char * const value)
{
  initialize();
  openDb();

  const char *field = Plow::getField(key)->first;
  FieldType type    = Plow::getField(key)->second;

  char *quotedValue = sqlite3_mprintf("%q", value);

  ostringstream select;

  int numberValue;
  char *endptr;

  switch (key)
  {
    case 'N': case 'P': case 'R':
      mUpdateAlbum += ",\n\t";
      mUpdateAlbum += field;
      mUpdateAlbum += "='";
      mUpdateAlbum += quotedValue;
      mUpdateAlbum += "'";
    break;

    default:
      switch(type)
      {
        case Plow::TABLE:
          select << "SELECT id_" << field << " FROM tbl_" << field
                 << " WHERE " << field << "='" << quotedValue << "';";

          mSqlite3->exe(select.str().c_str());

          if(mSqlite3->rows() == 0)
          {
            mUpdateValues.push_back(
                pair<char, const char *> (key, value));
          }
          else
          {
            mUpdate += ",\n\t_id_";
            mUpdate += field;
            mUpdate += "=";
            mUpdate += mSqlite3->get(0,0);
          }
        break;

        case NUMBER:
          errno = 0;
          numberValue = strtol(quotedValue, &endptr, 10);
          if(endptr[0] || errno)
          {
            sqlite3_free(quotedValue);
            throw PlowException("appendUpdate", "can't convert to int");
          }
          mUpdate += ",\n\t";
          mUpdate += field;
          mUpdate += "='";
          mUpdate += quotedValue;
          mUpdate += "'";
        break;

        default:
          mUpdate += ",\n\t";
          mUpdate += field;
          mUpdate += "='";
          mUpdate += quotedValue;
          mUpdate += "'";
      }
  }

  sqlite3_free(quotedValue);
}



void Plow::appendFilter(string const &filter)
{
  mFilter.append(filter);
}



void Plow::createFilter(const string &select)
{
  char *tmp = new char[strlen(SELECT) + mMusicDir.size() + 1];

  sprintf(tmp, SELECT, mMusicDir.c_str());

  mFilter = tmp + select + "ORDER BY\n\t";

  delete[] tmp;

  string order = mIniParser->get("general/order");
  if(mShuffle)
  {
    order = "RANDOM()";
  }
  else if(order.empty())
  {
    order = "album ASC, part ASC, track ASC";
  }

  mFilter += order + ";";
}



void Plow::printTable()
{
  mSqlite3->exe(mQuery.c_str());

  if(mSqlite3->cols() > 0)
  {
    cout << "| ";
    int len = 0;
    for(int i = 0; i < mSqlite3->cols(); ++i)
    {
      len = strlen(mSqlite3->getHead(i))
                   - utf8strlen(mSqlite3->getHead(i));
      cout << setw(mSqlite3->getWidth(i) + len);
      cout << setiosflags(ios::left);
      cout << mSqlite3->getHead(i);
      cout << " | ";
    }
    cout << "\n" << endl;

    for(int i = 0; i < mSqlite3->rows(); i++)
    {
      cout << "| ";
      for(int j = 0; j < mSqlite3->cols(); j++)
      {
        len = strlen(mSqlite3->get(i, j))
                     - utf8strlen(mSqlite3->get(i, j));
        cout << setw(mSqlite3->getWidth(j) + len);
        cout << setiosflags(ios::left) << mSqlite3->get(i, j);
        cout << " | ";
      }
      cout << endl;
    }
  }
  else
  {
    cout << "> empty result" << endl;
  }
}



string Plow::createPlayerArgs()
{
  string inivalue = "general/player";
  inivalue.append(1, mPlayer);

  string player(mIniParser->get(inivalue.c_str()));

  if(player.empty())
  {
    PlowException e("Plow::createPlayerArgs");
    e.error() << "No player with number " << mPlayer
              << " set in inifile.\nUsing player0.";

    e.print();

    inivalue  = "general/player0";
    player    = mIniParser->get(inivalue.c_str());
    mPlayer = 0;
  }

  inivalue = "general/playernofork";
  if(mIniParser->get(inivalue.c_str()).find_first_of(mPlayer) !=
      string::npos)
  {
    mFork = false;
  }

  player += " \"";
  player += mPlaylistFile + "\"";

  return player;
}


int calcPercent(int total, float &percent)
{
  int   result = 1;
  float p      = 100.0 / total;
  float p1     = p;

  while(p < percent)
  {
    p += p1;
    ++result;
  }

  percent = p;

  return result;
}



void Plow::copy()
{
  auto_ptr<vector<string> > files(new vector<string>);

  string portable = mIniParser->get("general/portable");

  if(portable.empty())
  {
    errno = 0;
    throw PlowException("Plow::copy",
                        "no portable device set in config file");
  }

  cout << "> copying " << flush;

  ///
  /// read playlist and add files to files
  ///

  ifstream inputStream;
  inputStream.open(mPlaylistFile.c_str(), ios::in);
  if(!inputStream)
  {
    throw PlowException("Plow::copy", mPlaylistFile.c_str());
  }

  char *buf = new char[1024];

  int numFiles = 0;
  while(inputStream.getline(buf, 1024))
  {
    if(buf[0] != '#')
    {
      numFiles++;
      files->push_back(buf);
    }
  }
  inputStream.close();
  delete[] buf;


  cout << numFiles <<  " files to " << portable << " ...\n\t" << flush;

  string dst;

  ///
  /// find out how to rename files and copy them
  ///

  string portableName = mIniParser->get("general/portable_name");

  string query("SELECT *\n");
  query += FROM;
  query += WHERE;
  query += " AND file='";

  string q;

  float perc   = 5.0;
  int   numPer = calcPercent(files->size(), perc);
  int   count  = 0;

  for(unsigned int i = 0; i < files->size(); ++i)
  {
    dst = portable + "/";

    if(!portableName.empty())
    {
      q  = query + files->at(i).substr(mMusicDir.size()) + "';";
      openDb();
      mSqlite3->exe(q.c_str());

      if(mSqlite3->cols() == 0)
      {
        errno = 0;
        PlowException e("Plow::copy");
        e.error() << "file not found in database: " << files->at(i);
        e.print();
        continue;
      }

      dst += infoString(0,
                        StringParser(portableName.c_str()).getTokens(),
                        true);
    }
    else
    {
      /// fallback: use original filename
      dst += files->at(i).substr(mMusicDir.size());
    }

    if(copyfile(files->at(i), dst) == 1)
    {
      /// file already exists
    }

    if(++count % numPer == 0)
    {
      cout << (int)(count * perc / numPer) << " " << flush;
    }
  }

  if(count % numPer != 0)
  {
    cout << "100 ";
  }
  cout << "%" << endl;
}



void Plow::update()
{
  vector<pair<char, const char*> >::iterator it = mUpdateValues.begin();

  const char *field;
  const char *value;
  char *quotedValue;

  bool backedUp = false;

  string insert;
  ostringstream query;

  string filter="SELECT\n\tid_music\n  ";
  filter += FROM;
  filter += "  ";
  filter += WHERE;
  filter += mFilter;

  mSqlite3->exe((filter + ";").c_str());

  if(mSqlite3->rows() == 0)
  {
    printQuery(filter);

    cout << "> filter does not match" << endl;
    return;
  }

  while(it != mUpdateValues.end())
  {
    query.str("");

    field = Plow::getField(it->first)->first;
    value = it->second;

    ++it;

    quotedValue = sqlite3_mprintf("%q", value);

    query << "INSERT INTO tbl_" << field << " (" << field
          << ") VALUES ('" << quotedValue << "');";

    printQuery(query.str());

    query << "SELECT id_" << field << " FROM tbl_" << field
          << " WHERE " << field << "='" << quotedValue << "';";

    if(!backedUp)
    {
      backup();
      backedUp = true;
    }

    mSqlite3->exe(query.str().c_str());

    cout << "> created " << field << " " << value << endl;

    sqlite3_free(quotedValue);

    mUpdate += ",\n\t_id_";
    mUpdate += field;
    mUpdate += "=";
    mUpdate += mSqlite3->get(0,0);
  }

  string update;

  if(mUpdate.size() > 0)
  {
    update  ="UPDATE tbl_music SET ";
    update += &(mUpdate.c_str()[1]);
    update += "\nWHERE id_music IN (\n  ";
    update += filter + ");";

    if(!backedUp)
    {
      backup();
      backedUp = true;
    }

    printQuery(update);

    mSqlite3->exe(update.c_str());

    cout << "> " << mSqlite3->affectedRows()
         << " row(s) in tbl_music updated" << endl;
  }


  if(mUpdateAlbum.size() > 0)
  {
    update  = "UPDATE tbl_album SET ";
    update += &(mUpdateAlbum.c_str()[1]);
    update += "\nWHERE id_album IN (\n  ";
    update += filter + ");";

    if(!backedUp)
    {
      backup();
      backedUp = true;
    }

    printQuery(update);

    mSqlite3->exe(update.c_str());

    cout << "> " << mSqlite3->affectedRows()
         << " row(s) in tbl_album updated" << endl;
  }
}



string Plow::infoString(unsigned int row,
                        const vector<string *> &tokens,
                        bool removeslash)
{
  const string  forbidden = "\\\n\r\t\" '$@*{}[]()/:;&?";

  string out;
  string tmp;
  string tmp2;

  unsigned int strsize = 0;
  unsigned int pos;


  bool isField   = true;
  bool lastEmpty = false;

  for(unsigned int i = 0; i < tokens.size(); ++i)
  {
    tmp = "";
    isField = true;

    strsize = tokens[i]->size();

    ///
    /// first some 'special' fields
    ///
    if(tokens[i]->c_str()[0] == '['
        && tokens[i]->c_str()[strsize - 1] == ']')
    {
      if(*tokens[i] == "[track0]")
      {
        if(strlen(mSqlite3->get(row, "track")) < 2)
        {
          tmp += "0";
        }
        tmp += mSqlite3->get(row, "track");
      }

      else if(*tokens[i] == "[tracks0]")
      {
        if(strlen(mSqlite3->get(row, "tracks")) < 2)
        {
          tmp += "0";
        }
        tmp += mSqlite3->get(row, "tracks");
      }

      else if(*tokens[i] == "[part0]")
      {
        if(strlen(mSqlite3->get(row, "part")) < 2)
        {
          tmp += "0";
        }
        tmp += mSqlite3->get(row, "part");
      }

      else if(*tokens[i] == "[parts0]")
      {
        if(strlen(mSqlite3->get(row, "parts")) < 2)
        {
          tmp += "0";
        }
        tmp += mSqlite3->get(row, "parts");
      }

      else if(*tokens[i] == "[extension]")
      {
        tmp2 = mSqlite3->get(row, "file");
        pos = tmp2.rfind('.');
        if(pos != string::npos)
        {
          tmp += tmp2.substr(pos);
        }
      }

      else if(*tokens[i] == "[artistOrAlbum]")
      {
        if(strcmp(mSqlite3->get(row, "album_id_artist"), "1") != 0)
        {
          tmp += mSqlite3->get(row, "artist");
        }
        else
        {
          tmp += mSqlite3->get(row, "album");
        }
      }

      else if(*tokens[i] == "[albumOrArtist]")
      {
        if(strcmp(mSqlite3->get(row, "album_id_artist"), "1") != 0)
        {
          tmp += mSqlite3->get(row, "album");
        }
        else
        {
          tmp += mSqlite3->get(row, "artist");
        }
      }

      else if(*tokens[i] == "[albumOrEmpty]")
      {
        if(strcmp(mSqlite3->get(row, "album_id_artist"), "1") != 0)
        {
          tmp += mSqlite3->get(row, "album");
        }
        else
        {
          lastEmpty = true;
        }
      }

      else if(*tokens[i] == "[emptyOrAlbum]")
      {
        if(strcmp(mSqlite3->get(row, "album_id_artist"), "1") == 0)
        {
          tmp += mSqlite3->get(row, "album");
        }
        else
        {
          lastEmpty = true;
        }
      }

      else if(*tokens[i] == "[artistOrEmpty]")
      {
        if(strcmp(mSqlite3->get(row, "album_id_artist"), "1") != 0)
        {
          tmp += mSqlite3->get(row, "artist");
        }
        else
        {
          lastEmpty = true;
        }
      }

      else if(*tokens[i] == "[emptyOrArtist]")
      {
        if(strcmp(mSqlite3->get(row, "album_id_artist"), "1") == 0)
        {
          tmp += mSqlite3->get(row, "artist");
        }
        else
        {
          lastEmpty = true;
        }
      }

      ///
      /// now fields from database
      ///
      else
      {
        tmp2 = &(tokens[i]->c_str()[1]);
        tmp2.erase(tmp2.size() - 1, 1);
        if(mSqlite3->get(row, tmp2.c_str()) != NULL)
        {
          tmp += mSqlite3->get(row, tmp2.c_str());
        }
        else
        {
          isField = false;
          tmp += *tokens[i];
        }
      }
    }

    else if (!lastEmpty)
    {
      isField = false;
      tmp += *tokens[i];
    }

    else if (lastEmpty)
    {
      lastEmpty = false;
    }

    if(removeslash && isField)
    {
      /// create a legal file name
      replaceChars(forbidden, tmp);
      out.append(tmp);
    }
    else
    {
      out += tmp;
    }
  }

  return out;
}



void Plow::createList()
{
  mSqlite3->exe(mFilter.c_str());

  cout << "> " << flush;

  if(mSqlite3->rows() == 0)
  {
    mPlay = false;
    cout << "empty result" << endl;
    return;
  }
  else
  {
    cout << mSqlite3->rows() << " file(s) found";
  }

  _Ios_Openmode open_mode = ios::out;
  if(mAddToPlaylist)
  {
    open_mode = ios::app;
  }

  ofstream m3u(mPlaylistFile.c_str(), open_mode);
  if(!m3u)
  {
    throw PlowException("Plow::createList", "Can't open playlist file.");
  }

  if(!mAddToPlaylist)
  {
    m3u << "#EXTM3U" << endl;
  }

  unsigned int playtime       = 0;
  unsigned int total_playtime = 0;

  StringParser sp(mIniParser->get("general/extinf").c_str());

  bool extinf = true;

  if(mIniParser->get("general/extinf").empty())
  {
    extinf = false;
  }

  for(int i = 0; i < mSqlite3->rows(); ++i)
  {
    playtime = (unsigned int)(atof(mSqlite3->get(i, "length")));
    total_playtime += playtime;
    if(extinf)
    {
      m3u << "#EXTINF:" << infoString(i, sp.getTokens()) << endl;
    }
    m3u << mSqlite3->get(i, 0) << endl;
  }
  m3u.close();

  unsigned int hours = (total_playtime / 3600);
  unsigned int mins  = (total_playtime - (hours * 3600)) / 60;
  unsigned int secs  = (total_playtime - (hours * 3600)) - (mins * 60);

  cout << " (" << hours << "h " << mins << "m " << secs << "s)" << endl;
}



int Plow::readTags()
{
  ///
  /// get files
  ///
  char *path = new char[mInsert.size() + 1];
  strcpy(path, mInsert.c_str());

  auto_ptr<PrioQ> files(new PrioQ);

  StringParser sp(mIniParser->get("general/extensions").c_str());

  getFiles(*files, path, true, sp.getArgv());

  delete[] path;

  cout << "> " << files->size() << " file(s) found" << endl;

  if(files->size() == 0)
  {
    return 0;
  }

  /// clear table tbl_tmp
  mSqlite3->exe("DELETE FROM tbl_tmp; VACUUM;");

  ReaderSelector *selector;
  AbstractReader *reader;
  int            removePos = mMusicDir.size();
  string         query("BEGIN TRANSACTION;");
  string         file;
  char           *quoted;

  const char * const  format =
     "INSERT INTO tbl_tmp (tmp_file_id, tmp_file, tmp_title, tmp_artist, \
      tmp_album, tmp_part, tmp_parts, tmp_track, tmp_tracks,\
      tmp_length, tmp_date, tmp_genre, tmp_rating, tmp_mood,\
      tmp_situation, tmp_tempo, tmp_language, tmp_comment) VALUES (\
      '%q', '%q', '%q', '%q', '%q', '%q', '%q', '%q', '%q',\
      '%q', '%q', '%q', '%q', '%q', '%q', '%q', '%q', '%q');";

  cout << "> reading metadata ...\n\t" << flush;

  float perc = 5.0;
  int ii     = calcPercent(files->size(), perc);
  int count  = 0;
  int added  = 0;

  ///
  /// read tags
  ///
  while(!files->empty())
  {
    if(++count % ii == 0)
    {
      cout << (int)(count * perc / ii) << " " << flush;
    }

    file = files->top();
    delete[] files->top();
    files->pop();

    quoted = sqlite3_mprintf(
                 "SELECT id_music FROM tbl_music WHERE file='%q';",
                 file.substr(removePos).c_str());

    mSqlite3->exe(quoted);

    sqlite3_free(quoted);

    /// file is already in database
    if(mSqlite3->rows() == 1)
    {
      continue;
    }

    selector = new ReaderSelector(file.c_str());
    reader   = selector->reader();

    if(selector->fileType() != ReaderSelector::UNKNOWN)
    {
      if(!reader->mappingDone())
      {
        StrMap *test = mIniParser->getSection(reader->sectionName());
        reader->setMapping(*test);
        delete test;
      }

      reader->read();

      quoted = sqlite3_mprintf(
          format,
          reader->id(), &reader->file()[mMusicDir.size()], reader->title(),
          reader->artist(), reader->album(), reader->part(),
          reader->parts(), reader->track(), reader->tracks(),
          reader->length(), reader->date(), reader->genre(),
          reader->rating(), reader->mood(), reader->situation(),
          reader->tempo(), reader->language(), reader->comment());

      query += quoted;
      sqlite3_free(quoted);
      ++added;
    }

    if(selector->fileType() != ReaderSelector::UNKNOWN
       && reader->error())
    {
      PlowException e("Plow::readTags");
      e.error() << "Error (" << reader->error() << ") at " << file;
      e.print();
    }

    delete selector;
  }

  if(count % ii != 0)
  {
    cout << "100 " << flush;
  }

  cout << "%\n\t" << added << " new file(s) found" << endl;

  query += "COMMIT TRANSACTION;";

  mSqlite3->exe(query.c_str());

  return added;
}



void Plow::addNewValues()
{
  Plow::initFields();

  string query, query2, query3;
  char *quoted;
  Sqlite3 sql(mDbFile.c_str());

  map<char, pair<const char *, Plow::FieldType> >::iterator it =
      Plow::fields.begin();

  query3 = "BEGIN TRANSACTION;";

  while(it != Plow::fields.end())
  {
    if(it->second.second != Plow::TABLE)
    {
      ++it;
      continue;
    }

    cout << "> new " << it->second.first << "s ..." << flush;

    query  = "SELECT DISTINCT tmp_";
    query += it->second.first;
    query += " FROM tbl_tmp;";

    mSqlite3->exe(query.c_str());

    for(int j = 0; j < mSqlite3->rows(); ++j)
    {
      quoted = sqlite3_mprintf("%q", mSqlite3->get(j, 0));

      query  = "SELECT id_";
      query += it->second.first;
      query += " FROM tbl_";
      query += it->second.first;
      query += " WHERE ";
      query += it->second.first;
      query += "='";
      query += quoted;
      query += "';";

      sql.exe(query.c_str());

      if(sql.rows() == 0)
      {
        query2  = "INSERT INTO tbl_";
        query2 += it->second.first;
        query2 += " (";
        query2 += it->second.first;
        query2 += ")  VALUES ('";
        query2 += quoted;
        query2 += "');";
        query2 += query;

        sql.exe(query2.c_str());

        cout << "\n\t" << mSqlite3->get(j, 0);
      }

      query3 += "UPDATE tbl_tmp SET tmp_id_";
      query3 += it->second.first;
      query3 += "='";
      query3 += sql.get(0, 0);
      query3 += "' WHERE tmp_";
      query3 += it->second.first;
      query3 += "='";
      query3 += quoted;
      query3 += "';";

      sqlite3_free(quoted);
    }

    cout << endl;

    ++it;
  }

  query3 += "COMMIT TRANSACTION;";

  mSqlite3->exe(query3.c_str());


  ///
  /// add additional info to albums
  ///
  cout << "> add info to new albums ..." << endl;

  query  = "SELECT tmp_id_album, tmp_id_artist FROM ";
  query += "tbl_tmp GROUP BY tmp_id_artist, tmp_id_album ";
  query += "HAVING COUNT(*) > 3;";

  mSqlite3->exe(query.c_str());

  query = "BEGIN TRANSACTION;";

  for(int i = 0; i < mSqlite3->rows(); ++i)
  {
    query += "UPDATE tbl_album SET album_id_artist='";
    query += mSqlite3->get(i, "tmp_id_artist");
    query += "' WHERE id_album='";
    query += mSqlite3->get(i, "tmp_id_album");
    query += "';";
  }

  query += " COMMIT TRANSACTION;";

  mSqlite3->exe(query.c_str());

  mSqlite3->exe(
      "SELECT DISTINCT tmp_id_album, tmp_parts, tmp_tracks FROM tbl_tmp;");

  query = "BEGIN TRANSACTION;";

  for(int i = 0; i < mSqlite3->rows(); ++i)
  {
    query += "UPDATE tbl_album SET parts='";
    query += mSqlite3->get(i, "tmp_parts");
    query += "', tracks='";
    query += mSqlite3->get(i, "tmp_tracks");
    query += "' WHERE id_album='";
    query += mSqlite3->get(i, "tmp_id_album");
    query += "';";
  }

  query += "COMMIT TRANSACTION;";

  mSqlite3->exe(query.c_str());
}



void Plow::insertNewSongs()
{
  string query = "BEGIN TRANSACTION;";

  char   *quoted;

  const char * const format =
      "INSERT INTO tbl_music (file_id, file, title, _id_artist, _id_album,\
       part, track, length, _id_genre, _id_rating, _id_mood,\
       _id_situation, _id_tempo, _id_language, date, comment) VALUES (\
      '%q', '%q', '%q', '%q', '%q', '%q', '%q', '%q', '%q', '%q', '%q',\
      '%q', '%q', '%q', '%q', '%q');";

  mSqlite3->exe(
      "SELECT * FROM tbl_tmp ORDER BY tmp_album, tmp_part, tmp_track;");

  int i = 0;
  for(; i < mSqlite3->rows(); ++i)
  {
    quoted = sqlite3_mprintf(
        format,
        mSqlite3->get(i, "tmp_file_id"),
        mSqlite3->get(i, "tmp_file"),
        mSqlite3->get(i, "tmp_title"),
        mSqlite3->get(i, "tmp_id_artist"),
        mSqlite3->get(i, "tmp_id_album"),
        mSqlite3->get(i, "tmp_part"),
        mSqlite3->get(i, "tmp_track"),
        mSqlite3->get(i, "tmp_length"),
        mSqlite3->get(i, "tmp_id_genre"),
        mSqlite3->get(i, "tmp_id_rating"),
        mSqlite3->get(i, "tmp_id_mood"),
        mSqlite3->get(i, "tmp_id_situation"),
        mSqlite3->get(i, "tmp_id_tempo"),
        mSqlite3->get(i, "tmp_id_language"),
        mSqlite3->get(i, "tmp_date"),
        mSqlite3->get(i, "tmp_comment"));

    query += quoted;

    sqlite3_free(quoted);
  }

  query += "COMMIT TRANSACTION;";

  mSqlite3->exe(query.c_str());

  cout << "> " << i << " file(s) added" << endl;

  mSqlite3->exe("DELETE FROM tbl_tmp; VACUUM;");
}



void Plow::insert(const char * const path)
{
  mInsert = path;
}



void Plow::run()
{
  initialize();

  openDb();

  if(mUpdate.size() > 0
     || mUpdateAlbum.size() > 0
     || mUpdateValues.size() > 0)
  {
    update();
    return;
  }

  if(mQuery.size() > 0)
  {
    printTable();
    return;
  }

  if(mInsert.size() > 0)
  {
    if(readTags() > 0)
    {
      backup();
      addNewValues();
      insertNewSongs();
    }
    return;
  }

  string select(FROM);
  select += WHERE;
  select += mFilter;

  if(!mCopy || mFilter.size() > 0)
  {
    createFilter(select);
    printQuery(mFilter);
    createList();

  }

  if(mCopy)
  {
    copy();
    mPlay = false;
  }

  if(mPlay)
  {
    StringParser sp(createPlayerArgs().c_str());
    char **playerArgs = sp.getArgv();

    int child_pid = 0;

    if(mFork)
    {
      child_pid = fork();
    }

    PlowException e("Plow::run");

    switch(child_pid)
    {
      case 0: /// child
        execvp(playerArgs[0], playerArgs);
        e.error() << "Can't execute player (" << playerArgs[0] << ")";
        throw e;
      case -1: /// fork error
        e.error() << "can't fork() process";
        throw e;
    }
  }
}
