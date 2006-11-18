
#ifndef PLOW_SQLITE3_H
#define PLOW_SQLITE3_H

//#include <sqlite3.h> // maybe for sqlite3 error constants

#include "Sqlite3Result.h"

using namespace std;

class Sqlite3 {

  public:
    Sqlite3(const char *databaseName);

    Sqlite3Result *exe(const char *query);
    void           free_table(char **sqlresult);
    int            error();
    const char    *errmsg();

    ~Sqlite3();

  private:
    int         rc;
    const char *dbname;
    char       *errstr;
};

#endif
