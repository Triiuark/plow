/* vi:set enc=utf-8 ts=4 sw=4: */

/***********************************************************************
 *                                                                     *
 *  Copyright (C) 2006 -2010  René Bählkow <reba@projekt182.de>        *
 *                                                                     *
 *  This file is part of Plow.                                         *
 *                                                                     *
 *  Plow is free software; you can redistribute it and/or              *
 *  modify it under the terms of the GNU General Public License        *
 *  as published by the Free Software Foundation; either version 2     *
 *  of the License, or (at your option) any later version.             *
 *                                                                     *
 *  Plow is distributed in the hope that it will be useful,            *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      *
 *  GNU General Public License for more details.                       *
 *                                                                     *
 *  You should have received a copy of the GNU General Public License  *
 *  along with Plow.  If not, see <http://www.gnu.org/licenses/>       *
 *                                                                     *
 ***********************************************************************/

#ifndef PLOW_CONSTANTS_H
#define PLOW_CONSTANTS_H

///
/// constants for help stuff
///

const char
		* const USAGE =
				"plow\
 [-|+<c>[e|l|g] <str> [ <str>]*]* [--show]\n\
     ([--0|...|9] [--random] [--add] [--noplay] | [--set (<c> <str>)+])\n\
     | [--list <tbl>] | [--query <sql>] | [--copy] | [--insert [<path>]]\n\
     | [--dump] | [--dumpfull] | [--print [c][d][p]] | [--help] | [--version]";

const char
		* const HELP =
				"\
-|+<c>[e|l|g] <str>  -   search <c> for fields containing <str>\n\
                     +   search <c> for fields not containing <str>\n\
                     e   <str> matches exactly\n\
                     l   value in <c> is less than <str>\n\
                     g   value in <c> is greater than <str>\n\
                     <c> is one of:\n\
                         A (album),  a (artist),    c (comment),\n\
                         d (date),   f (file name), g (genre),\n\
                         i (id),     L (lyrics),    l (language),\n\
                         m (mood),   N (tracks),    n (track),\n\
                         P (parts),  p (part),      R (release),\n\
                         r (rating), S (status),    s (situation),\n\
                         T (title),  t (tempo),     x (length)\n\
--S|set <c> <str>    set field <c> to value <str> using actual filter,\n\
                     <c> is the same as above\n\
                     (you can change more than one field at a time)\n\
--0|...|9            select player number (set in configuration file)\n\
--r|random           shuffle playlist\n\
--a|add              append to current playlist\n\
--n|noplay           don't start a player\n\
--s|show             print out sql statement for filter or set option\n\
\n\
--l|list <tbl>       print out values for <tbl>\n\
                     <tbl> is one of album, artist, genre,     language,\n\
                                     mood,  rating, situation, tempo\n\
\n\
--q|query <sql>      execute statement <sql>\n\
\n\
--c|copy             copy all files in the current playlist\n\
                     to portable_device set in config file\n\
\n\
--i|insert [<path>]  add (supported) files to database\n\
                     if called without argument path from config file is\n\
                     used\n\
                     Note: <path> has to be absolute and has to start\n\
                           with path from config file\n\
\n\
--d|dump             print out a table dump (without create statements)\n\
--D|dumpfull         print out a full table dump (with create statements)\n\
\n\
--p|print [c][d][p]  print configuration values:\n\
                       none or c - configuration file name\n\
                       d         - database file name\n\
                       p         - playlist file name\n\
--h|help             print this message\n\
--v|version          print version\n\
\n\
See man plow for some detailed information.";

///
/// constants for sql stuff
///

const char
		* const SELECT =
				"SELECT\n\t'%s' || `file` AS `file`,\n\t `file_id`,\
 `artist`, `title`, `album`, `genre`, `language`, `mood`,\n\t `tempo`, `rating`,\
 `situation`, `part`, `track`, `parts`, `tracks`,\n\t `length`, `release`, `date`,\
 `status`, `comment`, `lyrics`\n";

const char
		* const FROM =
				"FROM\n\t`tbl_music`, `tbl_artist`, `tbl_album`,\
 `tbl_genre`, `tbl_rating`,\n\t`tbl_language`, `tbl_mood`, `tbl_situation`,\
 `tbl_tempo`\n";

const char
		* const WHERE =
				"WHERE\n\t`id_artist`=`_id_artist` AND\
 `id_album`=`_id_album`\n\tAND `id_genre`=`_id_genre` AND\
 `id_rating`=`_id_rating`\n\tAND `id_language`=`_id_language` AND\
 `id_mood`=`_id_mood`\n\tAND `id_situation`=`_id_situation` AND\
 `id_tempo`=`_id_tempo`\n";

///
/// constants for the first run setup
///

const char
		* const INI_FILE =
				"\
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
#order = \"`album` ASC, `part` ASC, `track` ASC\"\n\
\n\
\n\
### the player you wish to use, at minimum player0 is required\n\
player0 = mplayer -playlist\n\
#player1 = audacious\n\
#player2 = xmms\n\
#player3 = amarok -pl\n\
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
#portable_name = [artistOrAlbum] / [albumOrEmpty] - [part] - [track0] -\
 [emptyOrArtist] - [title] [extension]\n\
\n\
### information string for m3u playlist\n\
extinf = [length] \", \" [artist] \" - \" [title]\n\
\n\
### search only for files with an extension in this list\n\
#extensions = mp3 MP3 ogg OGG\n\
\n\
### number of database backups to keep\n\
#backups = 5\n\
\n\
######################################################################\n\
###\n\
### here you can set how your ogg/vorbis files are tagged\n\
### all keys have to be upper case\n\
###\n\
[vorbis]\n\
\n\
#album     = ALBUM\n\
#artist    = ARTIST\n\
#comment   = COMMENT\n\
#date      = DATE\n\
#genre     = GENRE\n\
#id        = ID\n\
#language  = LANGUAGE\n\
#lyrics    = LYRICS\n\
#mood      = MOOD\n\
#part      = DISCNUMBER\n\
#parts     = DISCS\n\
#rating    = RATING\n\
#release   = RELEASE\n\
#situation = SITUATION\n\
#tempo     = TEMPO\n\
#title     = TITLE\n\
#track     = TRACKNUMBER\n\
#tracks    = TRACKS\n\
\n\
\n\
######################################################################\n\
###\n\
### here you can set how your mp3 files are tagged\n\
###\n\
[id3v2]\n\
\n\
#album     = TALB\n\
#artist    = TPE1\n\
#comment   = COMM/\n\
#date      = TDRC\n\
#genre     = TCON\n\
#id        = UFID/*\n\
#language  = TLAN\n\
#lyrics    = TXXX/LYRICS\n\
#mood      = TXXX/MOOD\n\
#rating    = POPM/*\n\
#release   = TDRL\n\
#situation = TXXX/SITUATION\n\
#tempo     = TXXX/TEMPO\n\
#title     = TIT2\
";

const char
		* const DATABASE =
				" BEGIN TRANSACTION;\
CREATE TABLE `tbl_album` (\n\
  `id_album`        INTEGER NOT NULL PRIMARY KEY,\n\
  `album`           TEXT             DEFAULT '',\n\
  `album_id_artist` INTEGER NOT NULL DEFAULT 1,\n\
  `tracks`          INTEGER          DEFAULT 0,\n\
  `parts`           INTEGER          DEFAULT 1,\n\
  `release`         TEXT             DEFAULT ''\n\
);\n\
INSERT INTO `tbl_album` VALUES(1, '', 1, 0, 1, '');\n\
\n\
\n\
\n\
CREATE TABLE `tbl_artist` (\n\
  `id_artist` INTEGER NOT NULL PRIMARY KEY,\n\
  `artist`    TEXT             UNIQUE\n\
);\n\
INSERT INTO `tbl_artist` VALUES(1, '');\n\
\n\
\n\
\n\
CREATE TABLE `tbl_genre` (\n\
  `id_genre` INTEGER NOT NULL PRIMARY KEY,\n\
  `genre`    TEXT             UNIQUE\n\
);\n\
INSERT INTO `tbl_genre` VALUES(1, '');\n\
\n\
\n\
\n\
CREATE TABLE `tbl_language` (\n\
  `id_language` INTEGER NOT NULL PRIMARY KEY,\n\
  `language`    TEXT             UNIQUE\n\
);\n\
INSERT INTO `tbl_language` VALUES(1, '');\n\
\n\
\n\
\n\
CREATE TABLE `tbl_mood` (\n\
  `id_mood` INTEGER NOT NULL PRIMARY KEY,\n\
  `mood`    TEXT             UNIQUE\n\
);\n\
INSERT INTO `tbl_mood` VALUES(1, '');\n\
\n\
\n\
\n\
CREATE TABLE `tbl_music` (\n\
  `id_music`       INTEGER NOT NULL PRIMARY KEY,\n\
  `file_id`        TEXT             DEFAULT '',\n\
  `file`           TEXT    NOT NULL UNIQUE,\n\
  `title`          TEXT             DEFAULT '',\n\
  `_id_artist`     INTEGER NOT NULL DEFAULT 1,\n\
  `_id_album`      INTEGER NOT NULL DEFAULT 1,\n\
  `part`           INTEGER          DEFAULT 1,\n\
  `track`          INTEGER          DEFAULT 0,\n\
  `length`         INTEGER          DEFAULT 0,\n\
  `_id_genre`      INTEGER NOT NULL DEFAULT 1,\n\
  `_id_rating`     INTEGER NOT NULL DEFAULT 1,\n\
  `_id_mood`       INTEGER NOT NULL DEFAULT 1,\n\
  `_id_situation`  INTEGER NOT NULL DEFAULT 1,\n\
  `_id_tempo`      INTEGER NOT NULL DEFAULT 1,\n\
  `_id_language`   INTEGER NOT NULL DEFAULT 1,\n\
  `date`           TEXT             DEFAULT '',\n\
  `status`         TEXT             DEFAULT '',\n\
  `comment`        TEXT             DEFAULT '',\n\
  `lyrics`         TEXT             DEFAULT ''\n\
);\n\
\n\
\n\
\n\
CREATE TABLE `tbl_rating` (\n\
  `id_rating` INTEGER NOT NULL PRIMARY KEY,\n\
  `rating`    TEXT             UNIQUE\n\
);\n\
INSERT INTO `tbl_rating` VALUES(1, '');\n\
\n\
\n\
\n\
CREATE TABLE `tbl_situation` (\n\
  `id_situation` INTEGER NOT NULL PRIMARY KEY,\n\
  `situation`    TEXT             UNIQUE\n\
);\n\
INSERT INTO `tbl_situation` VALUES(1, '');\n\
\n\
\n\
\n\
CREATE TABLE `tbl_tempo` (\n\
  `id_tempo` INTEGER NOT NULL PRIMARY KEY,\n\
  `tempo`    TEXT             UNIQUE\n\
);\n\
INSERT INTO `tbl_tempo` VALUES(1, '');\n\
\n\
\n\
\n\
CREATE TABLE `tbl_tmp` (\n\
  `tmp_id`           INTEGER NOT NULL PRIMARY KEY,\n\
  `tmp_file_id`      TEXT             DEFAULT '',\n\
  `tmp_file`         TEXT             DEFAULT '',\n\
  `tmp_title`        TEXT             DEFAULT '',\n\
  `tmp_artist`       TEXT             DEFAULT '',\n\
  `tmp_album`        TEXT             DEFAULT '',\n\
  `tmp_part`         INTEGER          DEFAULT 1,\n\
  `tmp_parts`        INTEGER          DEFAULT 1,\n\
  `tmp_track`        INTEGER          DEFAULT 0,\n\
  `tmp_tracks`       INTEGER          DEFAULT 0,\n\
  `tmp_length`       INTEGER          DEFAULT 0,\n\
  `tmp_date`         TEXT             DEFAULT '',\n\
  `tmp_release`      TEXT             DEFAULT '',\n\
  `tmp_genre`        TEXT             DEFAULT '',\n\
  `tmp_rating`       TEXT             DEFAULT '',\n\
  `tmp_mood`         TEXT             DEFAULT '',\n\
  `tmp_situation`    TEXT             DEFAULT '',\n\
  `tmp_tempo`        TEXT             DEFAULT '',\n\
  `tmp_language`     TEXT             DEFAULT '',\n\
  `tmp_comment`      TEXT             DEFAULT '',\n\
  `tmp_lyrics`       TEXT             DEFAULT '',\n\
  `tmp_id_artist`    INTEGER NOT NULL DEFAULT 1,\n\
  `tmp_id_album`     INTEGER NOT NULL DEFAULT 1,\n\
  `tmp_id_genre`     INTEGER NOT NULL DEFAULT 1,\n\
  `tmp_id_rating`    INTEGER NOT NULL DEFAULT 1,\n\
  `tmp_id_mood`      INTEGER NOT NULL DEFAULT 1,\n\
  `tmp_id_situation` INTEGER NOT NULL DEFAULT 1,\n\
  `tmp_id_tempo`     INTEGER NOT NULL DEFAULT 1,\n\
  `tmp_id_language`  INTEGER NOT NULL DEFAULT 1\n\
);\n\
\n\
\n\
\n\
CREATE VIEW `view_std` AS\n\
  SELECT\n\
    `file`, `file_id`, `artist`, `title`, `album`, `genre`, `language`, `mood`,\n\
    `tempo`, `rating`, `situation`, `part`, `track`, `parts`, `tracks`,\n\
    `length`, `release`, `date`, `status`, `comment`, `lyrics`\n\
  FROM\n\
    `tbl_music`, `tbl_artist`, `tbl_album`, `tbl_genre`, `tbl_rating`,\n\
    `tbl_language`, `tbl_mood`, `tbl_situation`, `tbl_tempo`\n\
  WHERE\n\
    `id_artist`=`_id_artist` AND `id_album`=`_id_album`\n\
    AND `id_genre`=`_id_genre` AND `id_rating`=`_id_rating`\n\
    AND `id_language`=`_id_language` AND `id_mood`=`_id_mood`\n\
    AND `id_situation`=`_id_situation` AND `id_tempo`=`_id_tempo`\n\
  ORDER BY\n\
    `album` ASC, `part` ASC, `track` ASC;\n\
COMMIT;";

#endif
