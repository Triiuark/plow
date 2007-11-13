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

#include "VorbisReader.h"

#include <tag.h>
#include <vorbisfile.h>
#include <xiphcomment.h>
#include <audioproperties.h>

using namespace TagLib;

VorbisReader::VorbisReader(const char *fname, CStrMap *fields)
  : AbstractReader(fname, fields)
{
  mCSM_fields["id"       ] = "ID";
  mCSM_fields["title"    ] = "TITLE";
  mCSM_fields["artist"   ] = "ARTIST";
  mCSM_fields["album"    ] = "ALBUM";
  mCSM_fields["part"     ] = "DISCNUMBER";
  mCSM_fields["parts"    ] = "DISCS";
  mCSM_fields["track"    ] = "TRACKNUMBER";
  mCSM_fields["tracks"   ] = "TRACKS";
  mCSM_fields["genre"    ] = "GENRE";
  mCSM_fields["rating"   ] = "RATING";
  mCSM_fields["mood"     ] = "MOOD";
  mCSM_fields["situation"] = "SITUATION";
  mCSM_fields["tempo"    ] = "TEMPO";
  mCSM_fields["language" ] = "LANGUAGE";
  mCSM_fields["date"     ] = "DATE";
  mCSM_fields["comment"  ] = "COMMENT";

  CStrMapIt it = fields->begin();
  while(it != fields->end())
  {
    if(strcmp(it->second, "") != 0)
    {
      mCSM_fields[it->first] = it->second;
    }
    ++it;
  }

  Ogg::Vorbis::File f(mcs_fname);
  Ogg::XiphComment *tag;
  if((tag = f.tag()))
  {
    const Ogg::FieldListMap *flm = &tag->fieldListMap();

    CStrMapIt it = mCSM_fields.begin();

    const StringList *sl;

    while(it != mCSM_fields.end())
    {

      sl = &(*flm)[mCSM_fields[it->first]];

      if(!sl->isEmpty())
      {
        (*mSM_values)[it->first] = (*sl)[0].toCString(true);
      }

      ++it;
    }
  } else {
    mi_err = 1;
  }

  int len;
  char buff[16];
  if(f.audioProperties())
  {
    len = f.audioProperties()->length();
    sprintf(buff, "%d", len);
    (*mSM_values)["length"] = buff;
  }
}
