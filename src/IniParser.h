/* vi:set fenc=utf-8 ts=4 sw=4: */

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

#ifndef PLOW_INI_PARSER_H
#define PLOW_INI_PARSER_H

#include "types.h"

/**
 * @brief simple parser for INI like configuration files
 *
 * a configuration file is structured like this:
 * <pre>
 *   # comment
 *   [section]
 *   option  = value
 *   option2 = " a value containing spaces "
 * </pre>
 */
class IniParser
{
	public:
		/**
		 * creates a new IniParser of a given configuration file
		 *
		 * @param iniFile the configuration file name
		 *
		 * @throws PlowException on any error
		 */
		IniParser(const char * const iniFile);

		/**
		 * returns the value for the given @a option
		 *
		 * @param option the key you want to have the value for,
		 *               key has to be something like "section/option"
		 *
		 * @returns the value, or an empty string if @a option not found
		 */
		std::string const &get(const char * const option);

		/**
		 * get a map containing all key-value pairs from a whole section
		 *
		 * @param section the section name (without brackets)
		 *
		 * @returns a new StrMap with the key-value pairs from [section]
		 *          - section is stripped from keys
		 *          - has to be freed using delete
		 */
		StrMap *getSection(const char * const section);

		/**
		 * destroys the IniParser
		 */
		~IniParser();

	private:
		StrMap mOptions;
};

#endif
