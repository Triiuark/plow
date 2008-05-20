/***********************************************************************
* Copyright (C) 2006 - 2008 by René Bählkow                            *
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

#ifndef PLOW_PLOW_H
#define PLOW_PLOW_H

#include <string>

#include "PlowException.h"
#include "IniParser.h"
#include "Sqlite3.h"

class Plow
{
  public:

    /**
     * all the fields (except the indices) in the database belonging to
     * one of this types
     */
    enum FieldType
    {
      TABLE,
      TEXT,
      NUMBER
    };

    /**
     * a map with the chars used for filter and set arguments as keys
     * @param key a char that represents the field (chars used for
     *            filter and set arguments)
     *
     * @returns a pair with the field name in the database
     *          and its Fieldtype
     */
    static std::pair<const char *, FieldType> *getField(char key);

    /**
     * creates a new Plow object
     */
    Plow();

    /**
     * chooses what to
     * (update db, insert new songs or create a playlist ...)
     * call it after parsing ALL arguments
     */
    void run();

    /**
     * @param play start a player?
     */
    void setPlay(bool play);

    /**
     * @param copy copy playlist to portable device?
     */
    void setCopy(bool copy);

    /**
     * @param random shuffle playlist?
     */
    void setRandom(bool random);

    /**
     * @param show print out the sql statements for filter and set stuff?
     */
    void setShowQuery(bool show);

    /**
     * @param append add files to playlist?
     */
    void setAppend(bool append);

    /**
     * selects player set in config file
     *
     * @param player to use
     */
    void setPlayer(char player);

    /**
     * execute a sql query
     *
     * @param query to execute
     */
    void setQuery(const char * const query);

    /**
     * add a filter value
     *
     * @param filter
     */
    void appendFilter(std::string const &filter);

    /**
     * add an update value
     *
     * @param key   char that is the key for a database field
     * @param value the new value for that field
     */
    void appendUpdate(char key, const char * const value);

    /**
     * add files in path to database
     *
     * @param path a path to a dir containing music files
     */
    void insert(const char * const path);


    void dump(bool full = false);
  private:
    static void initFields();

    void backup();

    void copy();
    void printQuery(const std::string &query);
    void createFileNames();
    void initialize();
    void createList();
    void createFilter(const std::string &select);
    void openDb();
    void dumpTable(const char * const, bool full = false);
    std::string createPlayerArgs();
    std::string infoString(unsigned int row,
                           const std::vector<std::string *> &tokens,
                           bool removeslash = false);
    void printTable();
    void update();

    int readTags();
    void addNewValues();
    void insertNewSongs();

    static std::map<char, std::pair<const char *, FieldType> > sFields;

    std::auto_ptr<IniParser> mIniParser;
    std::auto_ptr<Sqlite3>   mSqlite3;

    std::string mConfigDir;
    std::string mConfFile;
    std::string mDataDir;
    std::string mDbFile;
    std::string mPlaylistFile;
    std::string mMusicDir;

    std::string mFilter;
    std::string mUpdate;
    std::string mUpdateAlbum;
    std::string mQuery;
    std::string mInsert;

    std::vector<std::pair<char, const char *> > mUpdateValues;

    int mKeepBackup;

    char mPlayer;

    bool mShowQuery;
    bool mPlay;
    bool mAddToPlaylist;
    bool mShuffle;
    bool mFork;
    bool mCopy;
    bool mDoNothing;
};


#endif