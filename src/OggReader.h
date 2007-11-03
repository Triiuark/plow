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

#ifndef PLOW_OGG_READER_H
#define PLOW_OGG_READER_H

#include "global.h"
#include "AbstractReader.h"

#include <string>
#include <map>



class OggReader : public AbstractReader
{
  public:
    OggReader(const char* fname, CStrMap &fields);

    ~OggReader();

    const char *getId();
    const char *getArtist();
    const char *getTitle();
    const char *getAlbum();
    const char *getPart();
    const char *getParts();
    const char *getTrack();
    const char *getTracks();
    const char *getGenre();
    const char *getYear();
    const char *getRating();
    const char *getComment();
    const char *getLength();

    const char *get(const char *field);

    int error() {return err;}

  private:
    int err;

    StrMap *mSM_values;
};

#endif
