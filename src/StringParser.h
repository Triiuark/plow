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

#ifndef PLOW_STRING_PARSER_H
#define PLOW_STRING_PARSER_H

#include <string>
#include <vector>

/**
 * @brief splits a string into tokens
 */
class StringParser
{
	public:
		/**
		 * creates a new StringParser
		 *
		 * @param str the string
		 * @param delim the delimiter
		 */
		StringParser(const char *str, const char delim = ' ');

		/**
		 * @returns a vector containing the tokens
		 */
		std::vector<std::string *> const &getTokens() const;

		/**
		 * @returns a char ** like argv of the tokens,
		 *          but it is 0 terminated so we know where it ends
		 *          (e.g. for use in execvp)
		 */
		char **getArgv();

		/**
		 * @returns the number of tokens
		 */
		unsigned int getSize() const;

		/**
		 * destroys the StringParser
		 */
		~StringParser();

	private:
		char **mArgv;
		std::vector<std::string *> mTokens;
};

#endif
