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
