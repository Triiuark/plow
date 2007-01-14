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

#ifndef PLOW_SQLITE3_H
#define PLOW_SQLITE3_H

//#include <sqlite3.h> // maybe for sqlite3 error constants

#include "Sqlite3Result.h"

using namespace std;

class Sqlite3 {

  public:
    Sqlite3(const char *databaseName);

    Sqlite3Result *exe(const char *query);
    void           free_table(char **sqlresult);
    int            error();
    const char    *errmsg();

    ~Sqlite3();

  private:
    int         rc;
    const char *dbname;
    char       *errstr;
};

#endif
