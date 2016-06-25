/* vi:set fenc=utf-8 ts=4 sw=4: */

/***********************************************************************
 *                                                                     *
 *  Copyright (C) 2006-2016 René Bählkow <triiuark@projekt182.de>      *
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

#include "IniParser.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <cerrno>

#include "helper.h"
#include "PlowException.h"

using namespace std;

IniParser::IniParser(const char * const iniFile)
{

	ifstream ini(iniFile);
	if(!ini) {
		PlowException e("IniParser");
		e.error() << "could not open " << iniFile;
		throw e;
	}

	const string whitespace(" \t");

	string buffer;
	string group;
	string value;

	char *option;

	size_t pos;
	size_t line = 0;

	while(getline(ini, buffer)) {
		++line;
		value = "";


		/// trim buffer
		pos = buffer.find_first_not_of(whitespace);
		if(pos != string::npos) {
			buffer = buffer.substr(pos);
		}
		pos = buffer.find_last_not_of(whitespace);
		if(pos != string::npos) {
			buffer = buffer.substr(0, pos + 1);
		}

		/// ignore empty lines and comments
		if(buffer.length() == 0 || buffer.c_str()[0] == '#') {
			continue;
		}


		/// get section
		if(buffer.c_str()[0] == '[') {
			if(buffer.c_str()[buffer.length() - 1] == ']') {
				group = &buffer[1];
				group[group.size() - 1] = '/';
			} else {
				PlowException e("IniParser");
				e.error() << "missing ']' in " << iniFile
						<< " at line " << line;
				throw e;
			}
		}

		/// get option - value pairs
		else {
			pos = buffer.find_first_of("=");

			if(pos == string::npos) {
				PlowException e("IniParser");
				e.error() << "missing '=' in " << iniFile
						<< " at line " << line;
				throw e;
			} else if(pos == 0) {
				PlowException e("IniParser");
				e.error() << "missing option in" << iniFile
						<< " at line " << line;
				throw e;
			}

			value = buffer.substr(pos + 1);
			buffer = buffer.substr(0, pos);
			pos = buffer.find_last_not_of(whitespace);
			buffer = buffer.substr(0, pos + 1);
			pos = value.find_first_not_of(whitespace);
			if(pos != string::npos) {
				value = value.substr(pos);
			}

			if(value.length() > 0) {
				option = new char[group.length() + buffer.length() + 1];
				sprintf(option, "%s%s", group.c_str(), buffer.c_str());

				mOptions[option] = value;
			}
		}
	}
	ini.close();
}

string const &IniParser::get(const char * const option)
{
	return mOptions[option];
}

StrMap *IniParser::getSection(const char * const section)
{
	StrMap *sectionMap = new StrMap;
	StrMapIt it = mOptions.begin();

	int len = strlen(section);

	while(it != mOptions.end()) {
		if(strncmp(section, it->first, len) == 0 && it->second.size()
				> 0) {
			(*sectionMap)[&it->first[len + 1]] = it->second;
		}
		++it;
	}
	return sectionMap;
}

IniParser::~IniParser()
{
	StrMapIt it = mOptions.begin();

	while(it != mOptions.end()) {
		if(it->second != "") {
			/// if it->second is empty, key wasn't created with new,
			/// so I can't delete it
			delete[] it->first;
		}
		++it;
	}
}
