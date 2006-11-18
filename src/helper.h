
#ifndef PLOW_HELPER_H
#define PLOW_HELPER_H

#include "global.h"

using namespace std;

void getFiles(p_queue *fnames, char* path, const bool recursive);

int mkdir_r(const string &path, int mode = 0777);

int copyfile(const string &src, const string &dst);

uint utf8strlen(const char *utf8str);

#endif
