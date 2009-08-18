#include "helper.h"

#include <iostream>
#include <fstream>

#include <queue>

#include <cstring>
#include <cerrno>

extern "C"
{
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
}

#include "PlowException.h"

using namespace std;

void getFiles(PrioQ &fnames, char *path, const bool recursive,
		char **extensions, int mode)
{
	struct dirent *dent;
	struct stat st;

	char *fname;
	char *extension;
	int i;
	DIR *dir = opendir(path);

	if(!dir) {
		closedir(dir);
		return;
	}

	if(path[strlen(path) - 1] == '/') {
		path[strlen(path) - 1] = 0;
	}

	bool added;

	while((dent = readdir(dir))) {
		if(strcmp(dent->d_name, ".") != 0 && strcmp(dent->d_name, "..")
				!= 0) {
			added = false;

			fname = new char[strlen(path) + 1 + strlen(dent->d_name)
					+ 1];
			sprintf(fname, "%s/%s", path, dent->d_name);

			if(lstat(fname, &st) == 0) {
				if(recursive && S_ISDIR(st.st_mode)) {
					getFiles(fnames, fname, recursive, extensions, mode);
				} else if(S_ISREG(st.st_mode)) {
					if(access(fname, mode) == 0) {
						if(extensions && extensions[0] != 0) {
							i = 0;
							extension = strrchr(fname, '.');
							if(extension) {
								while(extensions[i] != 0 && i < 20) {
									if(strcmp(&extension[1],
											extensions[i]) == 0) {
										fnames.push(fname);
										added = true;
										break;
									}
									++i;
								}
							}
						} else {
							fnames.push(fname);
							added = true;
						}
					}
				}
			}

			if(!added) {
				delete[] fname;
			}
		}
	}
	closedir(dir);
}

void mkdir_r(string const &path, int mode)
{
	string newPath = path;

	if(newPath[newPath.size() - 1] == '/') {
		newPath[newPath.size() - 1] = 0;
	}

	struct stat st;

	if(newPath.length() == 0 || stat(newPath.c_str(), &st) == 0) {
		if(!S_ISDIR(st.st_mode)) {
			throw PlowException("mkdir_r", newPath.c_str(),
					"(Re)moving this file may help.");
		}
		/// path exists
		/// TODO: has it mode?
		return;
	}

	size_t pos = newPath.find_last_of('/');

	if(pos != string::npos && pos != 0) {
		string parentpath(newPath);
		parentpath.resize(pos);

		mkdir_r(parentpath, mode);
	}

	if(mkdir(newPath.c_str(), mode) == -1) {
		/// EEXIST never happens, its caught above
		throw PlowException("mkdir_r", path.c_str());
	}
}

int copyfile(string const &src, string const &dst)
{
	int err = 0;

	struct stat st;

	if(stat(dst.c_str(), &st) == 0) {
		/// file exists
		return 1;
	}

	string path(dst);
	path.resize(path.rfind('/'));
	mkdir_r(path);

	ifstream source;
	ofstream destination;

	source.open(src.c_str(), ios::binary | ios::in);
	if(!source) {
		throw PlowException("copyfile", src.c_str());
	}

	destination.open(dst.c_str(), ios::binary | ios::out);

	err = errno;
	if(!destination) {
		source.close();
		errno = err;
		throw PlowException("copyfile", dst.c_str());
	}

	char buffer[4096];
	int buffSize = sizeof(buffer);
	errno = 0;

	do {
		source.read(buffer, buffSize);
		destination.write(buffer, source.gcount());
	} while(source.good());

	if(source.bad() || !source.eof()) {
		err = errno;
		source.close();
		destination.close();
		unlink(dst.c_str());
		errno = err;
		throw PlowException("copyfile", src.c_str());
	}

	err = errno;

	source.close();
	destination.close();

	if(err != 0) {
		unlink(dst.c_str());
		errno = err;
		throw PlowException("copyfile", src.c_str());
	}

	return 0;
}

unsigned int utf8strlen(const char * const utf8str)
{
	unsigned int len = strlen(utf8str);
	unsigned int rl = 0;
	unsigned int i = 0;

	while(i < len) {
		if((utf8str[i++] & 0xc0) != 0x80) {
			/// byte don't starts with 10b so it is not a following byte
			++rl;
		}
	}

	return rl;
}

void replaceChars(const string &chars, string &in, char by)
{
	char *out = new char[in.length() + 1];
	bool replaced = false;
	unsigned int i = 0;
	unsigned int j = 0;

	while(in.c_str()[i] != 0) {
		if(chars.find(in.c_str()[i]) == string::npos) {
			replaced = false;
			out[j++] = in.c_str()[i];
		} else if(!replaced) {
			replaced = true;
			out[j++] = by;
		}
		++i;
	}

	out[j] = 0;

	in = out;
	delete[] out;
}
