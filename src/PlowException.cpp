#include "PlowException.h"

#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>

using namespace std;

PlowException::PlowException(const char * const function,
		const char * const error, const char * const solution)
{
	mErrno = errno;

	errno = 0;

	mFunction = function;

	if(error != 0) {
		mError << error;
	}

	if(solution != 0) {
		mSolution = solution;
	}
}

PlowException::PlowException(PlowException const &e)
{
	mErrno = e.err();
	mFunction = e.function();
	mSolution = e.solution();

	mError << e.errorStr();
}

std::ostringstream &PlowException::error()
{
	return mError;
}

void PlowException::print() const
{
	cout << endl;
	cerr << message() << endl;

	if(!mSolution.empty()) {
		cout << "\n" << mSolution << endl;
	}
}

string PlowException::message() const
{
	string out("Error in ");

	out.append(mFunction);
	out.append(": ");
	out.append(mError.str());

	if(mErrno != 0) {
		out.append(": ");
		out.append(strerror(mErrno));
	}

	return out;
}

int PlowException::err() const
{
	return mErrno;
}

string const &PlowException::function() const
{
	return mFunction;
}

string PlowException::errorStr() const
{
	return mError.str();
}

string const &PlowException::solution() const
{
	return mSolution;
}
