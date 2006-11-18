
#ifndef PLOW_GLOBAL_H
#define PLOW_GLOBAL_H

#include <cstring>
#include <map>
#include <queue>

using namespace std;

struct _cstrcmp {
  bool operator() (const char *a, const char *b) {
    return strcmp(a, b) < 0 ? true : false;
  }
};

struct _cstrcmp_r {
  bool operator() (const char *a, const char *b) {
    return strcmp(b, a) < 0 ? true : false;
  }
};

typedef map<const char*, const char*, _cstrcmp> a_array_cs;
typedef map<const char*, const char*, _cstrcmp>::iterator a_array_cs_it;

typedef map<const char*, string,      _cstrcmp> a_array_s;
typedef map<const char*, string,      _cstrcmp>::iterator a_array_s_it;

typedef map<const char*, int,         _cstrcmp> a_array_i;

typedef priority_queue<char *, vector<char *>, _cstrcmp_r> p_queue;

#endif
