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

#include "helper.h"

#include <iostream>
#include <fstream>

#include <queue>

#include <cstring>
#include <cerrno>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "PlowException.h"

using namespace std;

void getFiles(PrioQ &fnames,
              char *path,
              const bool recursive,
              char **extensions,
              int mode)
{
  struct dirent *dent;
  struct stat    st;

  char *fname;
  char *extension;
  int  i;
  DIR  *dir = opendir(path);

  if(!dir) {
    closedir(dir);
    return;
  }

  if(path[strlen(path) - 1] == '/') {
    path[strlen(path) - 1] = 0;
  }

  while((dent = readdir(dir))) {

    if(strcmp(dent->d_name, ".") != 0
        && strcmp(dent->d_name, "..") != 0 ) {

      fname = new char[strlen(path) + 1 + strlen(dent->d_name) + 1];
      sprintf(fname, "%s/%s", path, dent->d_name);

      if(lstat(fname, &st) == 0) {
        if(recursive && S_ISDIR(st.st_mode)) {
          getFiles(fnames, fname, recursive, extensions, mode);
          delete[] fname;
        } else if(S_ISREG(st.st_mode)) {
          if(access(fname, mode) == 0) {
            if(extensions && extensions[0] != 0) {
              i = 0;
              extension = strrchr(fname, '.');
              if(extension) {
                while(extensions[i] != 0 && i < 9999) {
                  cout << extension << ": " << extensions[i] << endl;
                  if(strcmp(&extension[1], extensions[i]) == 0) {
                    fnames.push(fname);
                  }
                  ++i;
                }
              }
            } else {
              fnames.push(fname);
            }
          }
        } else {
          delete[] fname;
        }
      } else {
        delete[] fname;
      }
    }
  }
  closedir(dir);
}



void mkdir_r(const string &path, int mode)
{
  struct stat st;

  if(path.length() == 0 || stat(path.c_str(), &st) == 0) {
    if(!S_ISDIR(st.st_mode)) {
      throw PlowException("mkdir_r",
                          path.c_str(),
                          "(Re)moving this file may help.");
    }
    // path exists
    // TODO: has it mode?
    return;
  }

  uint pos = path.find_last_of('/');

  if(pos != string::npos && pos != 0) {
    string parentpath(path);
    parentpath.resize(pos);

    mkdir_r(parentpath);
  }

  if(mkdir(path.c_str(), mode) == -1) {
    // EEXIST never happens, its caught above
    throw PlowException("mkdir_r", path.c_str());
  }
}



int copyfile(const string &src, const string &dst)
{
  int err = 0;

  struct stat st;

  if(stat(dst.c_str(), &st) == 0) {
    // file exists
    return 1;
  }

  string path(dst);
  path.resize(path.rfind('/'));
  mkdir_r(path);

  ifstream source;
  ofstream destination;
  source.open(src.c_str(), ios::binary|ios::in);
  if(!source) {
    throw PlowException("copyfile", src.c_str());
  }

  destination.open(dst.c_str(), ios::binary|ios::out);

  err = errno;
  if(!destination) {
    source.close();
    errno = err;
    throw PlowException("copyfile", dst.c_str());
  }
  char c;
  errno = 0;
  while(source.get(c)) {
    destination.put(c);
    if(errno != 0) {
      err = errno;
      source.close();
      destination.close();
      unlink(dst.c_str());
      errno = err;
      throw PlowException("copyfile", dst.c_str());
    }
  }
  err = errno;

  source.close();
  destination.close();

  if(err != 0) {
    unlink(dst.c_str());
    errno = err;
    throw PlowException("copyfile", src.c_str());
  }

  return 0;
}



uint utf8strlen(const char *utf8str)
{
  uint len = strlen(utf8str);
  uint rl  = 0;
  uint i   = 0;

  while(i < len){
    rl++;

    if(utf8str[i] > 0) {
      i += 1;
    } else if(utf8str[i] < -32) {
      i += 2;
    } else if(utf8str[i] < -16) {
      i += 3;
    } else if(utf8str[i] <  -8) {
      i += 4;
    } else if(utf8str[i] <  -4) {
      i += 5;
    } else if(utf8str[i] <  -2) {
      i += 6;
    }
  }

  return rl;
}



void replaceChars(const string &chars, string &in, char by) {
  char out[in.length() + 1];
  bool replaced = false;
  uint i        = 0;
  uint j        = 0;

  while(in.c_str()[i] != 0) {
    if(chars.find(in.c_str()[i]) == string::npos) {
      replaced = false;
      out[j++] = in.c_str()[i];
    } else if(!replaced) {
      replaced = true;
      out[j++] = by;
    }
    i++;
  }

  out[j] = 0;

  in = out;
}
