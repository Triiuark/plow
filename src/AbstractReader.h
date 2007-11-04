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

#ifndef PLOW_ABSTRACT_READER_H
#define PLOW_ABSTRACT_READER_H

class AbstractReader
{
  public:
    virtual const char *getId()        = 0;
    virtual const char *getTitle()     = 0;
    virtual const char *getArtist()    = 0;
    virtual const char *getAlbum()     = 0;
    virtual const char *getPart()      = 0;
    virtual const char *getParts()     = 0;
    virtual const char *getTrack()     = 0;
    virtual const char *getTracks()    = 0;
    virtual const char *getGenre()     = 0;
    virtual const char *getRating()    = 0;
    virtual const char *getMood()      = 0;
    virtual const char *getSituation() = 0;
    virtual const char *getTempo()     = 0;
    virtual const char *getLanguage()  = 0;
    virtual const char *getDate()      = 0;
    virtual const char *getComment()   = 0;
    virtual const char *getLength()    = 0;

    virtual const char *get(const char * field) = 0;

    virtual int error() = 0;

    virtual ~AbstractReader() {};
};

#endif
