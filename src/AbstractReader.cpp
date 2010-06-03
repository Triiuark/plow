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

#include "AbstractReader.h"

AbstractReader::AbstractReader(const char * const fname)
{
	mErr = 0;
	mValues["file"] = fname;
}

const char * const AbstractReader::album()
{
	return mValues["album"].c_str();
}

const char * const AbstractReader::artist()
{
	return mValues["artist"].c_str();
}

const char * const AbstractReader::comment()
{
	return mValues["comment"].c_str();
}

const char * const AbstractReader::date()
{
	return mValues["date"].c_str();
}

const char * const AbstractReader::file()
{
	return mValues["file"].c_str();
}

const char * const AbstractReader::genre()
{
	return mValues["genre"].c_str();
}

const char * const AbstractReader::id()
{
	return mValues["id"].c_str();
}

const char * const AbstractReader::language()
{
	return mValues["language"].c_str();
}

const char * const AbstractReader::length()
{
	return mValues["length"].c_str();
}

const char * const AbstractReader::lyrics()
{
	return mValues["lyrics"].c_str();
}

const char * const AbstractReader::mood()
{
	return mValues["mood"].c_str();
}

const char * const AbstractReader::part()
{
	return mValues["part"].c_str();
}

const char * const AbstractReader::parts()
{
	return mValues["parts"].c_str();
}

const char * const AbstractReader::rating()
{
	return mValues["rating"].c_str();
}

const char * const AbstractReader::release()
{
	return mValues["release"].c_str();
}

const char * const AbstractReader::situation()
{
	return mValues["situation"].c_str();
}

const char * const AbstractReader::tempo()
{
	return mValues["tempo"].c_str();
}

const char * const AbstractReader::title()
{
	return mValues["title"].c_str();
}

const char * const AbstractReader::track()
{
	return mValues["track"].c_str();
}

const char * const AbstractReader::tracks()
{
	return mValues["tracks"].c_str();
}

int AbstractReader::error() const
{
	return mErr;
}

AbstractReader::~AbstractReader()
{
	/// empty
}
