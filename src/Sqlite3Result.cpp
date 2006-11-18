
#include "Sqlite3Result.h"

#include <vector>
#include <map>
#include <cstring>

#include <sqlite3.h>

#include "global.h"
#include "helper.h"

using namespace std;
/* Implemetations */

Sqlite3Result::Sqlite3Result(char **sqlresult, int rows, int cols) {
  result = sqlresult;
  nrows  = rows;
  ncols  = cols;

  columns  = new a_array_i;
  head     = new vector<const char *>;

  colWidth = NULL;

  for(int i = 0; i < ncols; i++) {
    head->push_back(result[i]);
    (*columns)[result[i]] = i + 1;
  }
}

const char *Sqlite3Result::getHead(int col) {
  if(col >= 0 && col < ncols) {
    return head->at(col);
  }
  return NULL;
}

const char *Sqlite3Result::get(int row, int col) {
  int field = ((row + 1) * ncols) + col;
  if(field >= ncols && field < (nrows + 1)*ncols) {
    return result[field];
  }
  return NULL;
}

const char *Sqlite3Result::get(int row, const char *column) {
  return get(row, (*columns)[column] - 1);
}

int Sqlite3Result::rows() {
  return nrows;
}

int Sqlite3Result::cols() {
  return ncols;
}

int Sqlite3Result::getWidth(int col) {
  if(colWidth == NULL) {
    colWidth = new map<int, int>;
    uint len, tmp;
    for(int i = 0; i < ncols; i++) {
      len = 0;
      for(int j = 0; j < (nrows + 1); j++) {
        tmp = utf8strlen(result[j * ncols + i]);
        if(tmp > len) {
          len = tmp;
        }
      }
      (*colWidth)[i] = len;
    }
  }

  return (*colWidth)[col];
}


Sqlite3Result::~Sqlite3Result() {
  sqlite3_free_table(result); result = NULL;
  delete columns; columns = NULL;
  delete head; head = NULL;
  if(colWidth != NULL) {
    delete colWidth; colWidth = NULL;
  }
}
