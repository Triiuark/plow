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

#include "ID3Reader.h"

#include <string>

#include <tag.h>
#include <audioproperties.h>
#include <mpegfile.h>
#include <id3v2tag.h>
#include <textidentificationframe.h>

#include "global.h"

using namespace TagLib;



ID3Reader::ID3Reader(const char* fname, CStrMap &fields)
{
    mSM_values = new StrMap;
    ID3v2::Tag *tag;
    MPEG::File f(fname);
    if((tag = f.ID3v2Tag())) {

      (*mSM_values)["artist" ] = tag->artist ().to8Bit(true);
      (*mSM_values)["title"  ] = tag->title  ().to8Bit(true);
      (*mSM_values)["album"  ] = tag->album  ().to8Bit(true);
      (*mSM_values)["genre"  ] = tag->genre  ().to8Bit(true);
      (*mSM_values)["comment"] = tag->comment().to8Bit(true);

      if(tag->year() > 0) {
        (*mSM_values)["year"] = "" + tag->year();
      }

      std::string s;
      ID3v2::TextIdentificationFrame *tif;
      unsigned int pos;

      tif = (ID3v2::TextIdentificationFrame *)tag->frameListMap()["TRCK"][0];
      if(tif) {
        s = tif->toString().to8Bit(true);
        pos = s.find('/', 0);
        if(pos == std::string::npos) {
          (*mSM_values)["track"] = s;
        } else {
          (*mSM_values)["track"]  = s.substr(0, pos);
          (*mSM_values)["tracks"] = s.substr(++pos, s.size() - 1);
        }
      }

      tif = (ID3v2::TextIdentificationFrame *)tag->frameListMap()["TPOS"][0];
      if(tif) {
        s = tif->toString().to8Bit(true);
        pos = s.find('/', 0);
        if(pos == std::string::npos) {
          (*mSM_values)["part"] = s;
        } else {
          (*mSM_values)["part"]  = s.substr(0, pos);
          (*mSM_values)["parts"] = s.substr(++pos, s.size() - 1);
        }
      }
    }

    if(f.audioProperties()) {
      int len = f.audioProperties()->length() * 1000;
      char buff[16];
      sprintf(buff, "%d", len);
      //buff[15] = 0;
      (*mSM_values)["length"] = (buff);
  }
}




const char *ID3Reader::getId()
{
  return (*mSM_values)["id"].c_str();
}

const char *ID3Reader::getArtist()
{
  return (*mSM_values)["artist"].c_str();
}

const char *ID3Reader::getTitle()
{
  return (*mSM_values)["title"].c_str();
}

const char *ID3Reader::getAlbum()
{
  return (*mSM_values)["album"].c_str();
}

const char *ID3Reader::getPart()
{
  return (*mSM_values)["part"].c_str();
}

const char *ID3Reader::getParts()
{
  return (*mSM_values)["parts"].c_str();
}

const char *ID3Reader::getTrack()
{
  return (*mSM_values)["track"].c_str();
}

const char *ID3Reader::getTracks()
{
  return (*mSM_values)["tracks"].c_str();
}

const char *ID3Reader::getGenre()
{
  return (*mSM_values)["genre"].c_str();
}

const char *ID3Reader::getYear()
{
  return (*mSM_values)["year"].c_str();
}

const char *ID3Reader::getRating()
{
  return (*mSM_values)["rating"].c_str();
}

const char *ID3Reader::getComment()
{
  return (*mSM_values)["comment"].c_str();
}

const char *ID3Reader::getLength()
{
  return (*mSM_values)["length"].c_str();
}

const char *ID3Reader::get(const char *field)
{
  return (*mSM_values)[field].c_str();
}



ID3Reader::~ID3Reader()
{
  delete mSM_values;
}
