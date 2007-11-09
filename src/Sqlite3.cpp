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

#include "Sqlite3.h"

#include <iostream>
#include <sqlite3.h>

#include "Sqlite3Result.h"

#include "PlowException.h"

using namespace std;

Sqlite3::Sqlite3(const char *databaseName)
{
  mi_err = 0;
  mcs_dbname = databaseName;
}



Sqlite3Result *Sqlite3::exe(const char *query)
{
  sqlite3       *db;
  Sqlite3Result *rs = 0;
  char          **result;
  char          *zErrMsg;
  int           nrow;
  int           ncol;

  mi_err = sqlite3_open(mcs_dbname, &db);

  if(mi_err == SQLITE_OK) {
    mi_err = sqlite3_get_table(db,
                                 query,
                                 &result,
                                 &nrow,
                                 &ncol,
                                 &zErrMsg);
    if(mi_err == SQLITE_OK) {
      rs = new Sqlite3Result(result, nrow, ncol);
    }

    if(zErrMsg) {
      throw PlowException("Sqlite3::exe", zErrMsg);
    }

  } else {
    throw PlowException("Sqlite3::exe", sqlite3_errmsg(db));
  }
  sqlite3_close(db);

  return rs;
}



int Sqlite3::error()
{
  return mi_err;
}




Sqlite3::~Sqlite3()
{
  // empty
}
