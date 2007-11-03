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

#ifndef PLOW_PLOW_EXCEPTION_H
#define PLOW_PLOW_EXCEPTION_H

#include <string>

/**
 * @brief main exception class of plow
 */
class PlowException
{
  public:
    /**
     * creates a new PlowException with the given messages
     * @param where    the function name, where the error occurs
     * @param what     what exactly is the problem
     * @param solution a hint to solve the problem, if there is one
     */
    PlowException(const char *where,
                  const char *what,
                  const char *solution = 0);

    /**
     * @returns a string containing an error description
     */
    std::string message();

    /**
     * prints message() to std::cerr
     */
    void print();

    /**
     * @returns the value of errno at the moment
     *          of creating this PlowException
     */
    int error();

    /**
     * destroys the PlowException
     */
    ~PlowException();

  private:
    int mi_err;
    std::string mcs_where;
    std::string mcs_what;
    std::string mcs_solution;
};

#endif
