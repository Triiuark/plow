
#include "Sqlite3.h"

#include <iostream>
#include <sqlite3.h>

#include "Sqlite3Result.h"

using namespace std;
/* Implementations */

Sqlite3::Sqlite3(const char *databaseName) {
  dbname = databaseName;
  errstr = NULL;
  rc     = 0;
}

Sqlite3Result * Sqlite3::exe(const char *query) {
  sqlite3        *db;
  Sqlite3Result  *rs = NULL;
  char          **result;
  char           *zErrMsg;
  int             nrow;
  int             ncol;

  delete[] errstr; errstr = NULL;

  rc = sqlite3_open(dbname, &db);

  if(rc == SQLITE_OK) {
    rc = sqlite3_get_table(db, query, &result, &nrow, &ncol, &zErrMsg);
    if(rc == SQLITE_OK) {
      rs = new Sqlite3Result(result, nrow, ncol);
    }
    if(zErrMsg) {
      errstr = new char[strlen(zErrMsg) + 11];
      sprintf(errstr, "SQL error %s", zErrMsg);
      free(zErrMsg);
      // string errorstring(errstr);
      // throw errorstring;
    }
  } else {
    const char *tmp = sqlite3_errmsg(db);
    errstr = new char[strlen(tmp) + 1];
    sprintf(errstr, "%s", tmp);
  }
  sqlite3_close(db);
  return rs;
}

int Sqlite3::error() {
  return rc;
}

const char *Sqlite3::errmsg() {
  return errstr;
}


Sqlite3::~Sqlite3() {
  delete[] errstr; errstr = NULL;
}
