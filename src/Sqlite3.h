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

#include "Sqlite3Result.h"

using namespace std;

/**
 * @brief handles sqlite 3 connection
 */
class Sqlite3
{
  public:
    /**
     * creates a new Sqlite3 object with the given @a databaseName
     *
     * @param databaseName the database file name
     */
    Sqlite3(const char *databaseName);

    /**
     * executes a sql statement and returns the resulting table as a
     * Sqlite3Result object
     *
     * @param query the sql statement to execute
     *
     * @throws PlowException on any error
     *
     * @returns a new Sqlite3Result that has to be freed,
     *          using @a delete
     */
    Sqlite3Result *exe(const char *query);

    /**
     * @returns a sqlite3 error code (sqlite3.h),
     *          SQLITE_OK means no error
     */
    int error();

    /**
     * destroys the Sqlite3 object
     */
    ~Sqlite3();

  private:
    int         mi_error;
    const char *mcs_dbname;
};

#endif
