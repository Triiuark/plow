
#ifndef PLOW_SQLITE3_RESULT_H
#define PLOW_SQLITE3_RESULT_H

#include <vector>
#include <map>

#include "global.h"

using namespace std;

class Sqlite3Result {

  public:
    Sqlite3Result(char **sqlresult, int rows, int cols);

    const char *getHead(int col);
    const char *get(int row, int col);
    const char *get(int row, const char *column);

    int rows();
    int cols();

    int getWidth(int col);

    ~Sqlite3Result();

  private:
    char **result;
    int    ncols;
    int    nrows;

    vector<const char *> *head;
    a_array_i            *columns;
    map<int, int>        *colWidth;
};

#endif
