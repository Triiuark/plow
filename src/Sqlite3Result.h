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

#ifndef PLOW_SQLITE3_RESULT_H
#define PLOW_SQLITE3_RESULT_H

#include <vector>
#include <map>

#include "global.h"

using namespace std;

class Sqlite3Result {

  public:
    Sqlite3Result(char **sqlresult, int rows, int cols);

    const char *getHead(int col);
    const char *get(int row, int col);
    const char *get(int row, const char *column);

    int rows();
    int cols();

    int getWidth(int col);

    ~Sqlite3Result();

  private:
    char **result;
    int    ncols;
    int    nrows;

    vector<const char *> *head;
    a_array_i            *columns;
    map<int, int>        *colWidth;
};

#endif
