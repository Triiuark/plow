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

#include "AbstractReader.h"

class TagReader {
  public:

    static const enum {
      UNKNOWN = -1,
      OGG_VORBIS,
      MPEG
    } FILE_TYPE;

    TagReader(const char* fname);

    ~TagReader() {
      if(t) {
        delete t; t = 0;
      }
    }

    const char *id()      {return t->getId();     }
    const char *artist()  {return t->getArtist(); }
    const char *title()   {return t->getTitle();  }
    const char *album()   {return t->getAlbum();  }
    const char *part()    {return t->getPart();   }
    const char *parts()   {return t->getParts();  }
    const char *track()   {return t->getTrack();  }
    const char *tracks()  {return t->getTracks(); }
    const char *genre()   {return t->getGenre();  }
    const char *rating()  {return t->getRating(); }
    const char *year()    {return t->getYear();   }
    const char *comment() {return t->getComment();}
    const char *length()  {return t->getLength(); }

    int fileType() {return type;}

    int error(){return 0;}

  private:
    AbstractReader *t;
    int type;
};

#endif
