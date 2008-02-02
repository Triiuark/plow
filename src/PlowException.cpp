#include "PlowException.h"

#include <iostream>
#include <string>
#include <cerrno>

#include "types.h"

using namespace std;

PlowException::PlowException(const char *where,
                             const char *what,
                             const char *solution)
{
  mi_err = errno;
  if(errno != 0)
  {
    errno = 0;
  }
  mcs_where = where;
  mcs_what  = what;

  if(solution != 0)
  {
    mcs_solution = solution;
  }
}



void PlowException::print() const
{
  cout << endl;
  cerr << message() << endl;
  cout << endl;
}



string PlowException::message() const
{
  string out("Error in ");

  out.append(mcs_where);
  out.append(":\n  >");
  out.append(mcs_what);

  if(mi_err != 0)
  {
    out.append(":\n  >");
    out.append(strerror(mi_err));
  }

  if(!mcs_solution.empty())
  {
    out.append("\n  >");
    out.append(mcs_solution);
  }

  return out;
}



int PlowException::error() const
{
  return mi_err;
}



PlowException::~PlowException()
{
  // empty
}
