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
