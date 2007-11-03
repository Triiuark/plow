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

#include "OggReader.h"

#include <tag.h>
#include <vorbisfile.h>
#include <xiphcomment.h>
#include <audioproperties.h>

#include "global.h"

using namespace TagLib;

OggReader::OggReader(const char* fname, CStrMap &fields)
{
  err    = 0;
  mSM_values = new StrMap;
  Ogg::Vorbis::File f(fname);
  Ogg::XiphComment *tag;
  if((tag = f.tag())) {
    const Ogg::FieldListMap *flm = &tag->fieldListMap();

    CStrMapIt it = fields.begin();

    const StringList *sl;

    while(it != fields.end()) {

      sl = &(*flm)[fields[it->first]];

      if(!sl->isEmpty()) {
        (*mSM_values)[it->first] = (*sl)[0].to8Bit(true);
      }

      ++it;
    }
  } else {
    err = 1;
  }

  if(f.audioProperties()) {
    int len = f.audioProperties()->length() * 1000;
    char *buff = new char[16];
    sprintf(buff, "%d", len);
    (*mSM_values)["length"] = (buff);
    delete[] buff;
  }
}

const char *OggReader::getId()
{
  return (*mSM_values)["id"].c_str();
}

const char *OggReader::getArtist()
{
  return (*mSM_values)["artist"].c_str();
}

const char *OggReader::getTitle()
{
  return (*mSM_values)["title"].c_str();
}

const char *OggReader::getAlbum()
{
  return (*mSM_values)["album"].c_str();
}

const char *OggReader::getPart()
{
  return (*mSM_values)["part"].c_str();
}

const char *OggReader::getParts()
{
  return (*mSM_values)["parts"].c_str();
}

const char *OggReader::getTrack()
{
  return (*mSM_values)["track"].c_str();
}

const char *OggReader::getTracks()
{
  return (*mSM_values)["tracks"].c_str();
}

const char *OggReader::getGenre()
{
  return (*mSM_values)["genre"].c_str();
}

const char *OggReader::getRating()
{
  return (*mSM_values)["rating"].c_str();
}

const char *OggReader::getYear()
{
  return (*mSM_values)["year"].c_str();
}

const char *OggReader::getComment()
{
  return (*mSM_values)["comment"].c_str();
}

const char *OggReader::getLength()
{
  return (*mSM_values)["length"].c_str();
}

const char *OggReader::get(const char *field)
{
  return (*mSM_values)[field].c_str();
}



OggReader::~OggReader()
{
  delete mSM_values;
}
