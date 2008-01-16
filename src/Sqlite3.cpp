#include "Sqlite3.h"

#include <iostream>
#include <sqlite3.h>

#include "Sqlite3Result.h"

#include "PlowException.h"

using namespace std;

Sqlite3::Sqlite3(const char *databaseName)
{
  mi_err = 0;
  mcs_dbname = databaseName;
}



Sqlite3Result *Sqlite3::exe(const char *query)
{
  sqlite3       *db;
  Sqlite3Result *rs = 0;
  char          **result;
  char          *zErrMsg;
  int           nrow;
  int           ncol;

  mi_err = sqlite3_open(mcs_dbname, &db);

  if(mi_err == SQLITE_OK)
  {
    mi_err = sqlite3_get_table(db,
                               query,
                               &result,
                               &nrow,
                               &ncol,
                               &zErrMsg);
    if(mi_err == SQLITE_OK)
    {
      rs = new Sqlite3Result(result, nrow, ncol);
    }

    if(zErrMsg)
    {
      throw PlowException("Sqlite3::exe", zErrMsg, query);
    }

  } else {
    throw PlowException("Sqlite3::exe", sqlite3_errmsg(db), query);
  }
  sqlite3_close(db);

  return rs;
}



int Sqlite3::error()
{
  return mi_err;
}



Sqlite3::~Sqlite3()
{
  // empty
}
