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

//
// constants for help stuff
//

const char *USAGE = "plow\
 [-|+A|a|g|l|m|r|s|t|T[e] <str> [<str>]*]* [-Q]\n\
 ([-0...9] [-S] [--add] [--noplay] | [--set (A|a|g|l|m|r|s|t <str>)+])\n\
 | [-L <tbl>] | [-q <sql>] | [-C] | [-I <dir>] | [--help] | [--version]";

const char *HELP = "\
-|+<c>[e] <str>   - search for fields containing <str>                \n\
                  + search for fields not containing <str>            \n\
                  e <str> matches exactly                             \n\
                  <c> is one of A (album),     a (artist), g (genre), \n\
                                l (language),  m (mood),   r (rating),\n\
                                s (situation), t (tempo),  T (title)  \n\
--set <c> <str>   set field <c> to value <str> using actual filter,   \n\
                  <c> is the same as above, except T is not supported \n\
                  (you can change more than one field at a time)      \n\
-0...9            select player number set in configuration file      \n\
-S                shuffle playlist                                    \n\
--add             append to playlist                                  \n\
--noplay          don't start a player                                \n\
-Q                print out the sql statement                         \n\
                                                                      \n\
-L <tbl>          print out values for <tbl>                          \n\
                  <tbl> is one of album, artist, genre,     language, \n\
                                  mood,  rating, situation, tempo     \n\
                                                                      \n\
-q <sql>          execute statement <sql>                             \n\
                                                                      \n\
-C                copy all files in the current playlist              \n\
                  to portable_device set in config file               \n\
                                                                      \n\
-I <dir>          insert all (supported) files in <dir> into database \n\
                  Note: <dir> has to be an absolute path              \n\
                                                                      \n\
--help            print this message                                  \n\
--version         print version                                       \n\
                                                                      \n\
See man plow for some detailed information.";
/*
EXAMPLES                                                              \n\
----------------------------------------------------------------------\n\
plow -a queen                - artist contains queen                  \n\
plow -a queen -T rock        - artist contains queen                  \n\
                               AND title contains rock                \n\
plow -ae Queen               - artist is exactly Queen                \n\
plow +a queen                - artist NOT contains queen              \n\
plow +ae Queen               - artist IS NOT Queen                    \n\
plow -a queen beatles        - artist contains queen OR beatles       \n\
plow -a queen +a queens      - artist contains queen AND NOT queens   \n\
plow -a queen -a 'stone age' - artist contains queen AND stone age    \n\
                                                                      \n\
plow -a queen --set l english r 'best music ever'                     \n\
  - sets language = english and rating = 'best music ever'            \n\
    for all songs where artist contains queen                         \n\
";
*/

//
// constants for sql stuff
//

const char *SELECT = "SELECT\n\t'%s' || file as file, artist, title,\
 length, album, genre,\n\tlanguage, mood, tempo, rating, situation,\
 part, track,\n\tparts, tracks\n";

const char *FROM = "FROM\n\ttbl_music, tbl_artist, tbl_album,\
 tbl_genre, tbl_rating,\n\ttbl_language,tbl_mood, tbl_situation,\
 tbl_tempo\n";

const char *WHERE = "WHERE\n\tid_artist=_id_artist AND\
 id_album=_id_album\n\tAND id_genre=_id_genre AND\
 id_rating=_id_rating\n\tAND id_language=_id_language AND\
 id_mood=_id_mood\n\tAND id_situation=_id_situation AND\
 id_tempo=_id_tempo\n";

//
// constants for the first run setup
//

const char *INI_FILE ="\
######################################################################\n\
###\n\
### general options you have to set\n\
###\n\
\n\
[general]\n\
\n\
### full path to your 'overall' music folder\n\
#path = /path/to/music\n\
\n\
\n\
### complete path to your playlist\n\
#playlist = /path/to/plow.m3u\n\
\n\
\n\
### default output order (SQL 'ORDER BY' syntax)\n\
#order = \"album ASC, part ASC, track ASC\"\n\
\n\
\n\
### the player you wish to use, at minimum player0 is required\n\
player0 = mplayer -playlist\n\
player1 = audacious\n\
player2 = xmms\n\
player3 = amarok\n\
\n\
### the player numbers that requires not to fork\n\
playernofork = 0\n\
\n\
\n\
### full path to your portable music player\n\
#portable = /path/to/portable/player\n\
\n\
\n\
### filename on portable\n\
#portable_name = [artistOrAlbum] / [albumOrEmpty] - [part0] - [track0] -\
 [emptyOrArtist] - [title] [extension]\n\
\n\
### information string for m3u playlist\n\
extinf = [length] \", \" [artist] \" - \" [title]\n\
\n\
### search only for files with an extension in this list\n\
extensions = mp3 MP3 ogg OGG\n\
\n\
\n\
######################################################################\n\
###\n\
### in this section you can put abbrevations for often used\n\
### command line options\n\
### you can call these abbrevations using .NAME\n\
### where NAME is your abbrevation\n\
### you can use all command line options here\n\
###\n\
\n\
[abbr]\n\
### this one executes an sql statement, which selects all records\n\
### with different artists = sampler\n\
compilation = -q \"SELECT id_album, album FROM tbl_album WHERE\
 album_id_artist=1;\"\n\
\n\
### select all files where genre contains rock\n\
rock = -g rock\n\
\n\
\n\
######################################################################\n\
###\n\
### here you can set how your ogg/vorbis files are tagged\n\
###\n\
[vorbis]\n\
\n\
#title     = TITLE\n\
#artist    = ARTIST\n\
#album     = ALBUM\n\
#part      = DISCNUMBER\n\
#track     = TRACKNUMBER\n\
#genre     = GENRE\n\
#comment   = COMMENT\n\
#date      = DATE\n\
\n\
#id        = ID\n\
#parts     = DISCS\n\
#tracks    = TRACKS\n\
#rating    = RATING\n\
#mood      = MOOD\n\
#situation = SITUATION\n\
#tempo     = TEMPO\n\
#language  = LANGUAGE\n\
\n\
\n\
######################################################################\n\
###\n\
### here you can set how your mp3 files are tagged\n\
###\n\
[id3v2]\n\
\n\
#id        = UFID/*\n\
#comment   = COMM/\n\
#rating    = POPM/*\n\
#mood      = TXXX/MOOD\n\
#situation = TXXX/SITUATION\n\
#tempo     = TXXX/TEMPO\n\
#language  = TXXX/LANGUAGE\n\
";


const char *DATABASE ="\
CREATE TABLE tbl_album (\n\
  id_album        INTEGER NOT NULL PRIMARY KEY,\n\
  album           TEXT    NOT NULL DEFAULT '',\n\
  album_id_artist INTEGER NOT NULL DEFAULT 1,\n\
  tracks          INTEGER NOT NULL DEFAULT 0,\n\
  parts           INTEGER NOT NULL DEFAULT 1,\n\
  album_release   TEXT    NOT NULL DEFAULT ''\n\
);\n\
INSERT INTO 'tbl_album' VALUES(1, '', 1, 0, 1, 0);\n\
\n\
\n\
\n\
CREATE TABLE tbl_artist (\n\
  id_artist INTEGER NOT NULL PRIMARY KEY,\n\
  artist    TEXT    NOT NULL UNIQUE\n\
);\n\
INSERT INTO 'tbl_artist' VALUES(1, '');\n\
\n\
\n\
\n\
CREATE TABLE tbl_genre (\n\
  id_genre INTEGER NOT NULL PRIMARY KEY,\n\
  genre    TEXT    NOT NULL UNIQUE\n\
);\n\
INSERT INTO 'tbl_genre' VALUES(1, '');\n\
\n\
\n\
\n\
CREATE TABLE tbl_language (\n\
  id_language INTEGER NOT NULL PRIMARY KEY,\n\
  language    TEXT    NOT NULL UNIQUE\n\
);\n\
INSERT INTO 'tbl_language' VALUES(1, '');\n\
\n\
\n\
\n\
CREATE TABLE tbl_mood (\n\
  id_mood INTEGER NOT NULL PRIMARY KEY,\n\
  mood    TEXT    NOT NULL UNIQUE\n\
);\n\
INSERT INTO 'tbl_mood' VALUES(1, '');\n\
\n\
\n\
\n\
CREATE TABLE tbl_music (\n\
  id_music       INTEGER NOT NULL PRIMARY KEY,\n\
  file_id        TEXT    NOT NULL DEFAULT '',\n\
  file           TEXT    NOT NULL UNIQUE,\n\
  title          TEXT    NOT NULL DEFAULT '',\n\
  _id_artist     INTEGER NOT NULL DEFAULT 1,\n\
  _id_album      INTEGER NOT NULL DEFAULT 1,\n\
  part           INTEGER NOT NULL DEFAULT 1,\n\
  track          INTEGER NOT NULL DEFAULT 0,\n\
  length         INTEGER NOT NULL DEFAULT 0,\n\
  _id_genre      INTEGER NOT NULL DEFAULT 1,\n\
  _id_rating     INTEGER NOT NULL DEFAULT 1,\n\
  _id_mood       INTEGER NOT NULL DEFAULT 1,\n\
  _id_situation  INTEGER NOT NULL DEFAULT 1,\n\
  _id_tempo      INTEGER NOT NULL DEFAULT 1,\n\
  _id_language   INTEGER NOT NULL DEFAULT 1,\n\
  date           TEXT    NOT NULL DEFAULT '',\n\
  status         INTEGER NOT NULL DEFAULT 0,\n\
  comment        TEXT    NOT NULL DEFAULT '',\n\
  lyrics         TEXT    NOT NULL DEFAULT ''\n\
);\n\
\n\
\n\
\n\
CREATE TABLE tbl_rating (\n\
  id_rating INTEGER NOT NULL PRIMARY KEY,\n\
  rating    TEXT    NOT NULL UNIQUE\n\
);\n\
INSERT INTO 'tbl_rating' VALUES(1, '');\n\
\n\
\n\
\n\
CREATE TABLE tbl_situation (\n\
   id_situation INTEGER NOT NULL PRIMARY KEY,\n\
   situation    TEXT    NOT NULL UNIQUE\n\
);\n\
INSERT INTO 'tbl_situation' VALUES(1, '');\n\
\n\
\n\
\n\
CREATE TABLE tbl_tempo (\n\
  id_tempo INTEGER NOT NULL PRIMARY KEY,\n\
  tempo    TEXT    NOT NULL UNIQUE\n\
);\n\
INSERT INTO 'tbl_tempo' VALUES(1, '');\n\
\n\
\n\
\n\
CREATE TABLE tbl_tmp (\n\
   tmp_id           INTEGER NOT NULL PRIMARY KEY,\n\
   tmp_file_id      INTEGER NOT NULL DEFAULT 0,\n\
   tmp_file         TEXT    NOT NULL DEFAULT '',\n\
   tmp_title        TEXT    NOT NULL DEFAULT '',\n\
   tmp_artist       TEXT    NOT NULL DEFAULT '',\n\
   tmp_album        TEXT    NOT NULL DEFAULT '',\n\
   tmp_part         INTEGER NOT NULL DEFAULT 1,\n\
   tmp_parts        INTEGER NOT NULL DEFAULT 1,\n\
   tmp_track        INTEGER NOT NULL DEFAULT 0,\n\
   tmp_tracks       INTEGER NOT NULL DEFAULT 0,\n\
   tmp_length       INTEGER NOT NULL DEFAULT 0,\n\
   tmp_date         TEXT    NOT NULL DEFAULT '',\n\
   tmp_genre        TEXT    NOT NULL DEFAULT '',\n\
   tmp_rating       TEXT    NOT NULL DEFAULT '',\n\
   tmp_mood         TEXT    NOT NULL DEFAULT '',\n\
   tmp_situation    TEXT    NOT NULL DEFAULT '',\n\
   tmp_tempo        TEXT    NOT NULL DEFAULT '',\n\
   tmp_language     TEXT    NOT NULL DEFAULT '',\n\
   tmp_comment      TEXT    NOT NULL DEFAULT '',\n\
   tmp_lyrics       TEXT    NOT NULL DEFAULT '',\n\
   tmp_id_artist    INTEGER NOT NULL DEFAULT 1,\n\
   tmp_id_album     INTEGER NOT NULL DEFAULT 1,\n\
   tmp_id_genre     INTEGER NOT NULL DEFAULT 1,\n\
   tmp_id_rating    INTEGER NOT NULL DEFAULT 1,\n\
   tmp_id_mood      INTEGER NOT NULL DEFAULT 1,\n\
   tmp_id_situation INTEGER NOT NULL DEFAULT 1,\n\
   tmp_id_tempo     INTEGER NOT NULL DEFAULT 1,\n\
   tmp_id_language  INTEGER NOT NULL DEFAULT 1\n\
);";
