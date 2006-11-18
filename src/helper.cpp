
#include "helper.h"

#include <iostream>
#include <fstream>

#include <queue>

#include <cstring>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

using namespace std;

/*
 * puts every regular file in path into fnames,
 * if recursive it does for subfolders, too
 */
void getFiles(p_queue *fnames, char* path, const bool recursive) {

  struct dirent *dent;
  struct stat    st;

  char *fname;
  DIR  *dir = opendir(path);

  if(!dir) {
    closedir(dir);
    return;
  }

  if(path[strlen(path) - 1] == '/') {
    path[strlen(path) - 1] = 0;
  }

  while((dent = readdir(dir))) {
    if(strcmp(dent->d_name, ".") != 0 && strcmp(dent->d_name, "..") != 0 ) {
      fname = new char[strlen(path) + 1 + strlen(dent->d_name) + 1];
      sprintf(fname, "%s/%s", path, dent->d_name);

      if(lstat(fname, &st) == 0) {
        if(recursive && S_ISDIR(st.st_mode)) {
          getFiles(fnames, fname, recursive);
        } else if(S_ISREG(st.st_mode)) {
          fnames->push(fname);
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

/*
 * creates (recursive) the complete path and sets permission to mode
 */
int mkdir_r(const string &path, int mode) {
  struct stat st;
  if(path.size() == 0 || stat(path.c_str(), &st) == 0) {
    if(!S_ISDIR(st.st_mode)) {
      return -1;
    }
    return 0;
  }

  int  err = 0;
  uint pos = path.find_last_of('/');

  if(pos != string::npos && pos != 0) {
    string parentpath(path);
    parentpath.resize(pos);
    err = mkdir_r(parentpath);
    if(err) {
      return err;
    }
  }

  if(mkdir(path.c_str(), mode)) {
    err = errno; // EEXIST never happens!!!
  }
  return err;
}

/*
 * copies file src to dst (and creates folder if required)
 */
int copyfile(const string &src, const string &dst) {

  int err = 0;
  //cout << ">>>" << src << ": " << dst << endl;
  struct stat st;
  if(!stat(dst.c_str(), &st)){
    //cout << "File already exists: " << dst << endl;
    return 0;
  }

  string path(dst);
  path.resize(path.rfind('/'));

  err = mkdir_r(path);
  if(err) {
    //cout << "Fehler: " << strerror(err) << " / " << path2 << endl;
    return err;
  }

  ifstream source;
  ofstream destination;
  source.open(src.c_str(), ios::binary|ios::in);
  if(!source) {
    //cout << "Fehler:  öffnen source" << src << endl;
    return -1;

  }

  destination.open(dst.c_str(), ios::binary|ios::out);
  if(!destination) {
    //cout << strerror(errno) << "Fehler:  öffnen destination" << dst << endl;
    source.close();
    return -1;
  }

  char c;
  while(source.get(c)) {
    destination.put(c);
  }

  source.close();
  destination.close();
  return 0;
}


/*
 * calculates the real string length of utf-8 string utf8str
 * (returns number of characters instead of bytes)
 */
uint utf8strlen(const char *utf8str) {
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
