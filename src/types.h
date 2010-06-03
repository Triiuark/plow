/* vi:set enc=utf-8 ts=4 sw=4: */

/***********************************************************************
 *                                                                     *
 *  Copyright (C) 2006 -2010  René Bählkow <reba@projekt182.de>        *
 *                                                                     *
 *  This file is part of Plow.                                         *
 *                                                                     *
 *  Plow is free software; you can redistribute it and/or              *
 *  modify it under the terms of the GNU General Public License        *
 *  as published by the Free Software Foundation; either version 2     *
 *  of the License, or (at your option) any later version.             *
 *                                                                     *
 *  Plow is distributed in the hope that it will be useful,            *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      *
 *  GNU General Public License for more details.                       *
 *                                                                     *
 *  You should have received a copy of the GNU General Public License  *
 *  along with Plow.  If not, see <http://www.gnu.org/licenses/>       *
 *                                                                     *
 ***********************************************************************/

#ifndef PLOW_TYPES_H
#define PLOW_TYPES_H

#include <string>
#include <cstring>
#include <map>
#include <queue>

/**
 * for ascending sorting of STL containers with char* keys
 */
struct _cstrcmp
{
		bool operator()(const char * const a, const char * const b) const
		{
			return strcmp(a, b) < 0 ? true : false;
		}
};

/**
 * for descending sorting of STL containers with char* keys
 */
struct _cstrcmp_r
{
		bool operator()(const char * const a, const char * const b) const
		{
			return strcmp(b, a) < 0 ? true : false;
		}
};

/**
 * associative array of C strings
 */
typedef std::map<const char *, const char *, _cstrcmp> CStrMap;

/**
 * iterator for associative array of C strings
 */
typedef std::map<const char *, const char *, _cstrcmp>::iterator
		CStrMapIt;

/**
 * associative array of C++ strings
 */
typedef std::map<const char *, std::string, _cstrcmp> StrMap;

/**
 * iterator for associative array of C++ strings
 */
typedef std::map<const char *, std::string, _cstrcmp>::iterator
		StrMapIt;

/**
 * associative array of integers
 */
typedef std::map<const char *, int, _cstrcmp> IntMap;

/**
 * priority queue of vector<char *> with descending order
 */
typedef std::priority_queue<char *, std::vector<char *>, _cstrcmp_r>
		PrioQ;

#endif
