/***********************************************************************
* Copyright (C) 2006 by René Bählkow                                   *
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

#ifndef PLOW_TAG_READER_H
#define PLOW_TAG_READER_H

#include <map>

#include "global.h"
#include "AbstractReader.h"

class TagReader
{
  public:

    static const enum {
      UNKNOWN = -1,
      VORBIS,
      ID3V2
    } TAG_TYPE;

    TagReader(const char* fname, std::map<int, CStrMap *> fieldNames);

    const char *id();
    const char *artist();
    const char *title();
    const char *album();
    const char *part();
    const char *parts();
    const char *track();
    const char *tracks();
    const char *genre();
    const char *rating();
    const char *mood();
    const char *situation();
    const char *tempo();
    const char *language();
    const char *date();
    const char *comment();
    const char *length();

    int fileType();

    int error();

    ~TagReader();

  private:
    AbstractReader *mAR_reader;
    int            mi_type;
    int            mi_err;
};

#endif
