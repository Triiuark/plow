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

#ifndef PLOW_HELPER_H
#define PLOW_HELPER_H

#include "types.h"

using namespace std;

/**
 * puts every regular file with access mode @a mode in @a path into
 * @a fnames, if @a recursive it does for subfolders, too
 *
 * @param fnames     a queue to put found files in
 *                   (in descending order)
 * @param path       to search for files
 * @param recursive  if set, search in subfolders
 * @param extensions a list of file extensions (e.g. txt, ogg, mp3)
 *                   if not 0, only files with an extension that is in
 *                   @a extensions will be added
 *                   NOTE: @a extensions has to be 0 terminated
 * @param mode       access mode granted for files,
 *                   default: R_OK = readable @n
 *                   mode is a mask consisting of one or more of
 *                   R_OK, W_OK, X_OK and  F_OK.
 *                   (see: @a man @a access )
 */
void getFiles(PrioQ &fnames,
              char *path,
              const bool recursive,
              char **extensions = 0,
              int mode = R_OK);



/**
 * creates (recursive) the complete @a path
 * and sets permission to @a mode
 *
 * @param path to create
 * @param mode to use, default: 0755
 *
 * @throws PlowException on any error
 */
void mkdir_r(const string &path, int mode = 0755);



/**
 * copies file @a src to @a dst (and creates complete path)
 *
 * @param src source file name
 * @param dst destination file name
 *
 * @throws PlowException on any error
 *
 * @return 0 on success, 1 if @a dst exists
 */
int copyfile(const string &src, const string &dst);



/**
 * calculates the real string length of utf-8 string @a utf8str
 * (returns number of characters instead of bytes)
 *
 * @param utf8str utf-8 encoded string
 *
 * @return length in characters of @a utf8str
 */
uint utf8strlen(const char *utf8str);



/**
 * replaces every character listed in @a chars in @a in by @a by, but
 * a continious group of more than one character is replaced by only one
 * @a by
 *
 * @param chars a string containing all characters to replace
 * @param in the string where characters should be replaced
 * @param by charcter to split on
 */
void replaceChars(const string &chars, string &in, char by='_');

#endif
