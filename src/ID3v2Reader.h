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

#ifndef PLOW_ID3V2_READER_H
#define PLOW_ID3V2_READER_H

#include <id3v2tag.h>

#include "global.h"

#include "AbstractReader.h"

class ID3v2Reader : public AbstractReader
{
  public:
    ID3v2Reader(const char* fname, CStrMap *fields = 0);

    const char *getId();
    const char *getArtist();
    const char *getTitle();
    const char *getAlbum();
    const char *getPart();
    const char *getParts();
    const char *getTrack();
    const char *getTracks();
    const char *getGenre();
    const char *getRating();
    const char *getMood();
    const char *getSituation();
    const char *getTempo();
    const char *getLanguage();
    const char *getDate();
    const char *getComment();
    const char *getLength();



    const char *get(const char *field);



    int error();



    ~ID3v2Reader();

  private:
    int mi_err;
    StrMap *mSM_values;
    CStrMap mCSM_fields;

    void getTxxxFrame(const char *field,
                      const char *description,
                      TagLib::ID3v2::FrameList *fl);

    void getCommFrame(const char *field,
                      const char *description,
                      TagLib::ID3v2::FrameList *fl);

    void getUfidFrame(const char *field,
                      const char *description,
                      TagLib::ID3v2::FrameList *fl);

    void getPopmFrame(const char *field,
                      const char *description,
                      TagLib::ID3v2::FrameList *fl);
};

#endif
