#include "Sqlite3.h"

#include "helper.h"
#include "PlowException.h"

using namespace std;

Sqlite3::Sqlite3(const char * const databaseName)
{
  mErr  = 0;
  mRows = 0;
  mCols = 0;

  mHasResult = false;

  mDb  = NULL;

  mErr = sqlite3_open(databaseName, &mDb);

  if(mErr != SQLITE_OK)
  {
    throw PlowException("Sqlite3::Sqlite3", sqlite3_errmsg(mDb));
  }
}



void Sqlite3::exe(const char *query)
{
  char *zErrMsg;

  clear();

  mErr = sqlite3_get_table(mDb,
                           query,
                           &mData,
                           &mRows,
                           &mCols,
                           &zErrMsg);

  if(mErr != SQLITE_OK)
  {
    throw PlowException("Sqlite3::exe", zErrMsg, query);
  }

  mHasResult = true;

  mColumns.clear();
  mHead.clear();


  for(int i = 0; i < mCols; ++i)
  {
    mHead.push_back(mData[i]);
    (mColumns)[mData[i]] = i + 1;
  }
}



int Sqlite3::error() const
{
  return mErr;
}



const char * const Sqlite3::getHead(int col) const
{
  if(col >= 0 && col < mCols)
  {
    return mHead.at(col);
  }
  return 0;
}



const char * const Sqlite3::get(int row, int col)
{
  int field = ((row + 1) * mCols) + col;

  if(field >= mCols && field < (mRows + 1) * mCols)
  {
    return mData[field];
  }
  return 0;
}



const char * const Sqlite3::get(int row, const char * const column)
{
  return get(row, (mColumns)[column] - 1);
}



int Sqlite3::rows() const
{
  return mRows;
}



int Sqlite3::cols() const
{
  return mCols;
}



int Sqlite3::getWidth(int col, bool reset)
{
  if(mColWidth.empty() || reset)
  {
    mColWidth.clear();

    unsigned int len;
    unsigned int tmp;

    for(int i = 0; i < mCols; ++i)
    {
      len = 0;
      for(int j = 0; j < (mRows + 1); ++j)
      {
        tmp = utf8strlen(mData[j * mCols + i]);
        if(tmp > len)
        {
          len = tmp;
        }
      }
      (mColWidth)[i] = len;
    }
  }

  return (mColWidth)[col];
}



int Sqlite3::affectedRows() const
{
  return sqlite3_changes(mDb);
}



int Sqlite3::affectedRowsTotal() const
{
  return sqlite3_total_changes(mDb);
}


void Sqlite3::clear()
{
  if(mHasResult)
  {
    sqlite3_free_table(mData);
    mRows = 0;
    mCols = 0;
  }
  mHasResult = false;
}



Sqlite3::~Sqlite3()
{
  clear();
  sqlite3_close(mDb);
}
