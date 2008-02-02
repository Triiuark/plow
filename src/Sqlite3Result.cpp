#include "Sqlite3Result.h"

#include <vector>
#include <map>
#include <cstring>

#include <sqlite3.h>

#include "helper.h"

using namespace std;

Sqlite3Result::Sqlite3Result(char **sqlresult, int rows, int cols)
{
  m_result    = sqlresult;
  mi_rows     = rows;
  mi_cols     = cols;
  mIM_columns = new IntMap;
  m_head      = new vector<const char *>;
  m_colWidth  = 0;

  for(int i = 0; i < this->cols(); ++i)
  {
    m_head->push_back(m_result[i]);
    (*mIM_columns)[m_result[i]] = i + 1;
  }
}



const char *Sqlite3Result::getHead(int col) const
{
  if(col >= 0 && col < cols())
  {
    return m_head->at(col);
  }
  return 0;
}



const char *Sqlite3Result::get(int row, int col) const
{
  int field = ((row + 1) * cols()) + col;

  if(field >= cols() && field < (rows() + 1) * cols())
  {
    return m_result[field];
  }
  return 0;
}



const char *Sqlite3Result::get(int row, const char *column) const
{
  return get(row, (*mIM_columns)[column] - 1);
}



int Sqlite3Result::rows() const
{
  return mi_rows;
}



int Sqlite3Result::cols() const
{
  return mi_cols;
}



int Sqlite3Result::getWidth(int col)
{
  if(m_colWidth == 0)
  {
    // m_colWidth is empty, so calculate it for all columns
    m_colWidth = new map<int, int>;

    uint len;
    uint tmp;

    for(int i = 0; i < cols(); ++i)
    {
      len = 0;
      for(int j = 0; j < (rows() + 1); ++j)
      {
        tmp = utf8strlen(m_result[j * cols() + i]);
        if(tmp > len)
        {
          len = tmp;
        }
      }
      (*m_colWidth)[i] = len;
    }
  }

  return (*m_colWidth)[col];
}



Sqlite3Result::~Sqlite3Result()
{
  sqlite3_free_table(m_result);
  delete mIM_columns;
  delete m_head;
  if(m_colWidth != 0)
  {
    delete m_colWidth;
  }
}
