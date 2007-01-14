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

OggReader::OggReader(const char* fname, a_array_cs &fields) {
  err    = 0;
  values = new a_array_s;
  Ogg::Vorbis::File f(fname);
  Ogg::XiphComment *tag;
  if((tag = f.tag())) {
    const Ogg::FieldListMap *flm = &tag->fieldListMap();

    a_array_cs_it it = fields.begin();

    const StringList *sl;

    while(it != fields.end()) {

      sl = &(*flm)[fields[it->first]];

      if(!sl->isEmpty()) {
        (*values)[it->first] = (*sl)[0].to8Bit(true);
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
    (*values)["length"] = (buff);
    delete[] buff;
  }
}

const char *OggReader::getId()      {
  return (*values)["id"].c_str();
}

const char *OggReader::getArtist()  {
  return (*values)["artist"].c_str();
}

const char *OggReader::getTitle()   {
  return (*values)["title"].c_str();
}

const char *OggReader::getAlbum()   {
  return (*values)["album"].c_str();
}

const char *OggReader::getPart()    {
  return (*values)["part"].c_str();
}

const char *OggReader::getParts()   {
  return (*values)["parts"].c_str();
}

const char *OggReader::getTrack()   {
  return (*values)["track"].c_str();
}

const char *OggReader::getTracks()  {
  return (*values)["tracks"].c_str();
}

const char *OggReader::getGenre()   {
  return (*values)["genre"].c_str();
}

const char *OggReader::getRating()   {
  return (*values)["rating"].c_str();
}

const char *OggReader::getYear()    {
  return (*values)["year"].c_str();
}

const char *OggReader::getComment() {
  return (*values)["comment"].c_str();
}

const char *OggReader::getLength()  {
  return (*values)["length"].c_str();
}

const char *OggReader::get(const char *field) {
  return (*values)[field].c_str();
}
