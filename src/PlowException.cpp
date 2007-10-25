/***********************************************************************
* Copyright (C) 2007 by René Bählkow                                   *
*                                                                      *
* This program is free software; you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published by *
* the Free Software Foundation; either version 2 of the License, or    *
* (at your option) any later version.                                  *
*                                                                      *
* This program is distributed in the hope that it will be useful,      *
* but WITHOUT ANY WARRANTY; without even the implied warranty of       *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU General Public License for more details.                         *
*                                                                      *
* You should have received a copy of the GNU General Public License    *
* along with this program; if not, write to the                        *
* Free Software Foundation, Inc.,                                      *
* 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.            *
***********************************************************************/

#include "PlowException.h"

#include <iostream>
#include <cerrno>
#include <string>

#include "global.h"

using namespace std;

PlowException::PlowException(const char *where,
                             const char *what,
                             const char *solution) {
  mi_err = errno;
  if(errno != 0) {
    errno = 0;
  }
  mcs_where = where;
  mcs_what  = what;

  if(solution != 0) {
    mcs_solution = solution;
  }
}



PlowException::~PlowException() {
}



void PlowException::print() {
  cout << endl;
  cerr << message() << endl;
  cout << endl;
}



string PlowException::message() {

  string out("Error in ");

  out.append(mcs_where);
  out.append(":\n  >");
  out.append(mcs_what);

  if(mi_err != 0) {
    out.append(":\n  >");
    out.append(strerror(mi_err));
  }

  if(!mcs_solution.empty()) {
    out.append("\n  >");
    out.append(mcs_solution);
  }

  return out;
}



int PlowException::errn() {
  return mi_err;
}
