/************************************************************************
 * Copyright (C) 2006 - 2009 by René Bählkow                            *
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
 ************************************************************************/
#ifndef PLOW_SQLITE3_H
#define PLOW_SQLITE3_H

extern "C"
{
#include <sqlite3.h>
}

#include <map>
#include <vector>

#include "types.h"

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
		Sqlite3(const char * const databaseName);

		/**
		 * executes a sql statement and stores the resulting table
		 * internally
		 *
		 * @param query the sql statement to execute
		 *
		 * @throws PlowException on any error
		 */
		void exe(const char *query);

		/**
		 * @returns a sqlite3 error code (sqlite3.h),
		 *          SQLITE_OK means no error
		 */
		int error() const;

		/**
		 * @param col column number
		 *
		 * @return the column name of column col,
		 *         or 0 if col not found
		 */
		const char * const getHead(int col) const;

		/**
		 * @param row row number
		 * @param col column number
		 *
		 * @returns the value at row, col,
		 *          or 0 if row or col not found
		 */
		const char * const get(int row, int col);

		/**
		 * @param row    row number
		 * @param column column name
		 *
		 * @returns the value at row, column,
		 *          or 0 if row or column not found
		 */
		const char * const get(int row, const char * const column);

		/**
		 * @returns number of rows
		 */
		int rows() const;

		/**
		 * @returns number of columns
		 */
		int cols() const;

		/**
		 * @param col   column number
		 * @param reset if true reset and recalculate width
		 *
		 * @returns length (in utf-8 charcters, not bytes)
		 *          of longest string in column col
		 */
		int getWidth(int col, bool reset = false);

		/**
		 * @returns number of affected rows of most recently
		 *          INSERT, UPDATE or DELETE statement
		 */
		int affectedRows() const;

		/**
		 * @returns number of affected rows of INSERT, UPDATE or DELETE
		 *          statements since creation of an Sqlite3 object
		 */
		int affectedRowsTotal() const;

		/**
		 * destroys the Sqlite3 object
		 */
		~Sqlite3();

	private:
		void clear();

		int mErr;
		int mCols;
		int mRows;

		bool mHasResult;

		char **mData;

		sqlite3 *mDb;

		IntMap mColumns;
		std::vector<const char *> mHead;
		std::map<int, int> mColWidth;

};

#endif
