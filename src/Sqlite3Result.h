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

/**
 * @brief handles sqlite 3 result
 */
class Sqlite3Result
{
  public:
    /**
     * creates a new Sqlite3Result object
     *
     * @param sqlresult result value of sqlite3_get_table() function
     * @param rows      number of rows in result
     * @param cols      number of columns in result
     */
    Sqlite3Result(char **sqlresult, int rows, int cols);

    /**
     * @param col column number
     *
     * @return the column name of column col,
     *         or 0 if col not found
     */
    const char *getHead(int col);

    /**
     * @param row    row number
     * @param column column number
     *
     * @returns the value at row, col,
     *          or 0 if row or col not found
     */
    const char *get(int row, int col);

    /**
     * @param row    row number
     * @param column column name
     *
     * @returns the value at row, column,
     *          or 0 if row or column not found
     */
    const char *get(int row, const char *column);

    /**
     * @returns number of rows
     */
    int rows();

    /**
     * @returns number of columns
     */
    int cols();

    /**
     * @param col column number
     *
     * @returns lenght (in utf-8 charcters, not bytes)
     *          of longest string in column col
     */
    int getWidth(int col);

    /**
     * destroys the Sqlite3Result (frees also m_result)
     */
    ~Sqlite3Result();

  private:
    char **m_result;
    int  mi_cols;
    int  mi_rows;

    vector<const char *> *m_head;
    map<int, int>        *m_colWidth;
    IntMap               *mIM_columns;
};

#endif
