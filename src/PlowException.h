/***********************************************************************
* Copyright (C) 2006 - 2008 by René Bählkow                            *
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
#include <sstream>
#include <memory>

/**
 * @brief main exception class of plow
 */
class PlowException
{
  public:
    /**
     * creates a new PlowException with the given messages
     *
     * @param function the function name, where the error occurs
     * @param error    what "exactly" is the problem
     * @param solution a hint to solve the problem, if there is one
     */
    PlowException(const char * const function,
                  const char * const error = 0,
                  const char * const solution = 0);

    /**
     * creates a new PlowException from an existing one
     *
     * @param e an existing PlowException
     */
    PlowException(PlowException const &e);

    /**
     * @returns a string containing a complete error description
     */
    std::string message() const;

    /**
     * prints message() to std::cerr and the solution string to std::cout,
     * if there is one
     */
    void print() const;

    /**
     * @returns the value of errno at the moment
     *          of creating this PlowException
     */
    int err() const;

    /**
     * @returns ostringstream that contains the error description
     *          for easy creating of error messages
     */
    std::ostringstream &error();

    ///
    /// stuff for the copy ctor
    ///

    /**
     * @returns the function name where the error occurs
     */
    std::string const &function() const;

    /**
     * @returns the solution, if there was one
     */
    std::string const &solution() const;

    /**
     * @returns the error description
     */
    std::string errorStr() const;

  private:
    bool mUseErrno;
    int  mErrno;

    std::string mFunction;
    std::string mSolution;

    std::ostringstream mError;
};

#endif
