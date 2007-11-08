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

#include "TagReader.h"

#include "global.h"
#include "VorbisReader.h"
#include "ID3v2Reader.h"

TagReader::TagReader(const char *fname, map<int, CStrMap *> fieldNames)
    : mAR_reader(0), mi_type(UNKNOWN), mi_err(0)
{
  char head[5];
  head[4] = 0;
  FILE *f = fopen(fname, "r");
  fread(head, 1, 4, f);
  fclose(f);

  if(strncmp("OggS", head, 4) == 0) {
    mi_type    = VORBIS;
    mAR_reader = new VorbisReader(fname,
                                  fieldNames[TagReader::VORBIS]);
  } else if(strncmp("ID3", head, 3) == 0) {
    mi_type    = ID3V2;
    mAR_reader = new ID3v2Reader(fname, fieldNames[TagReader::ID3V2]);
  } else {
    mi_err = 0;
  }
}



const char *TagReader::id()        {return mAR_reader->getId();       }
const char *TagReader::artist()    {return mAR_reader->getArtist();   }
const char *TagReader::title()     {return mAR_reader->getTitle();    }
const char *TagReader::album()     {return mAR_reader->getAlbum();    }
const char *TagReader::part()      {return mAR_reader->getPart();     }
const char *TagReader::parts()     {return mAR_reader->getParts();    }
const char *TagReader::track()     {return mAR_reader->getTrack();    }
const char *TagReader::tracks()    {return mAR_reader->getTracks();   }
const char *TagReader::genre()     {return mAR_reader->getGenre();    }
const char *TagReader::rating()    {return mAR_reader->getRating();   }
const char *TagReader::mood()      {return mAR_reader->getMood();     }
const char *TagReader::situation() {return mAR_reader->getSituation();}
const char *TagReader::tempo()     {return mAR_reader->getTempo();    }
const char *TagReader::language()  {return mAR_reader->getLanguage(); }
const char *TagReader::date()      {return mAR_reader->getDate();     }
const char *TagReader::comment()   {return mAR_reader->getComment();  }
const char *TagReader::length()    {return mAR_reader->getLength();   }



int TagReader::fileType()
{
  return mi_type;
}



int TagReader::error()
{
  if(mAR_reader) {
    return mAR_reader->error();
  }
  return mi_err;
}



TagReader::~TagReader()
{
  if(mAR_reader) {
    delete mAR_reader;
  }
}
