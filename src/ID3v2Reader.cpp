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

#include "ID3v2Reader.h"

#include <sstream>


#include <tag.h>
#include <audioproperties.h>
#include <mpegfile.h>
#include <id3v2tag.h>
#include <commentsframe.h>
#include <textidentificationframe.h>
#include <uniquefileidentifierframe.h>
#include <unknownframe.h>
#include <tlist.h>

using namespace TagLib;

ID3v2Reader::ID3v2Reader(const char* fname, CStrMap *fields)
    : AbstractReader(fname, fields)
{
  mCSM_fields["comment"  ] = "COMM/";
  mCSM_fields["id"       ] = "UFID/*";
  mCSM_fields["rating"   ] = "POPM/*";
  mCSM_fields["mood"     ] = "TXXX/MOOD";
  mCSM_fields["situation"] = "TXXX/SITUATION";
  mCSM_fields["tempo"    ] = "TXXX/TEMPO";
  mCSM_fields["language" ] = "TXXX/LANGUAGE";

  CStrMapIt it = fields->begin();
  while(it != fields->end()) {
    if(strcmp(it->second, "") != 0) {
      mCSM_fields[it->first] = it->second;
    }
    ++it;
  }

  mSM_values = new StrMap;
  ID3v2::Tag *tag;
  MPEG::File f(mcs_fname);

  if((tag = f.ID3v2Tag())) {

    (*mSM_values)["artist"] = tag->artist().toCString(true);
    (*mSM_values)["title" ] = tag->title().toCString(true);
    (*mSM_values)["album" ] = tag->album().toCString(true);
    (*mSM_values)["genre" ] = tag->genre().toCString(true);

    if(tag->year() > 0) {
      char tmp[24];
      sprintf(tmp, "%d", tag->year());
      (*mSM_values)["date"] = tmp;
    }

    std::string s;
    ID3v2::TextIdentificationFrame *tif;
    unsigned int pos;

    if(!(tag->frameListMap()["TRCK"]).isEmpty()) {
      tif = (ID3v2::TextIdentificationFrame *)tag->frameListMap()["TRCK"][0];
      s = tif->toString().toCString(true);
      pos = s.find('/', 0);
      if(pos == std::string::npos) {
        (*mSM_values)["track"] = s;
      } else {
        (*mSM_values)["track"]  = s.substr(0, pos);
        (*mSM_values)["tracks"] = s.substr(++pos, s.size() - 1);
      }
    }

    if(!(tag->frameListMap()["TPOS"]).isEmpty()) {
      tif = (ID3v2::TextIdentificationFrame *)tag->frameListMap()["TPOS"][0];
      s = tif->toString().toCString(true);
      pos = s.find('/', 0);
      if(pos == std::string::npos) {
        (*mSM_values)["part"] = s;
      } else {
        (*mSM_values)["part"]  = s.substr(0, pos);
        (*mSM_values)["parts"] = s.substr(++pos, s.size() - 1);
      }
    }

    CStrMapIt fieldsIt = mCSM_fields.begin();
    const char *description;
    char frameName[5] = { 0, 0, 0, 0, 0 };
    while(fieldsIt != mCSM_fields.end()) {
      strncpy(frameName, fieldsIt->second, 4);
      description = strchr(fieldsIt->second, '/');
      if(description) {
        description = &description[1];
      } else {
        continue;
      }
      ID3v2::FrameList fl =
            (ID3v2::FrameList)(tag->frameListMap()[frameName]);
      if(!fl.isEmpty()) {
        if(strcmp(frameName, "TXXX") == 0) {
          getTxxxFrame(fieldsIt->first, description, &fl);
        }
        else if(strcmp(frameName, "COMM") == 0) {
          getCommFrame(fieldsIt->first, description, &fl);
        }
        else if(strcmp(frameName, "UFID") == 0) {
          getUfidFrame(fieldsIt->first, description, &fl);
        }
        else if(strncmp(fieldsIt->second, "POPM", 4) == 0) {
          getPopmFrame(fieldsIt->first, description, &fl);
        }
      }
      ++fieldsIt;
    }

  }
  int len;
  char buff[16];
  if(f.audioProperties()) {
    len = f.audioProperties()->length();
    sprintf(buff, "%d", len);
    (*mSM_values)["length"] = (buff);
  }
}



void ID3v2Reader::getTxxxFrame(const char *field,
                               const char *description,
                               ID3v2::FrameList *fl)
{
  char *tmp;

  ID3v2::UserTextIdentificationFrame *utif;

  for(TagLib::uint i = 0; i < fl->size(); ++i) {

    utif = (ID3v2::UserTextIdentificationFrame *)(*fl)[i];

    if(strcmp(utif->description().toCString(true), description) == 0) {

      char tmp2[strlen(utif->toString().toCString(true)) + 1];
      sprintf(tmp2, "%s", utif->toString().toCString(true));

      tmp = strchr(tmp2, ']');
      if(tmp && strlen(tmp) > 2) {
        (*mSM_values)[field] = &tmp[2];
      } else {
        (*mSM_values)[field] = tmp2;
      }

      return;
    }
  }
}



void ID3v2Reader::getCommFrame(const char *field,
                               const char *description,
                               ID3v2::FrameList *fl)
{
  ID3v2::CommentsFrame *comf;
  for(TagLib::uint i = 0; i < fl->size(); ++i) {
    comf = (ID3v2::CommentsFrame *)(*fl)[i];

    if(strcmp(comf->description().toCString(true), description) == 0) {

      char tmp[strlen(comf->toString().toCString(true)) + 1];
      sprintf(tmp, "%s", comf->toString().toCString(true));

      (*mSM_values)[field] = tmp;
      return;
    }
  }
}



void ID3v2Reader::getUfidFrame(const char *field,
                               const char *description,
                               ID3v2::FrameList *fl)
{
  char tmp[2];
  bool isBinary = false;
  std::ostringstream oss;
  ID3v2::UniqueFileIdentifierFrame *ufif;



  for(TagLib::uint i = 0; i < fl->size(); ++i) {
    ufif = (ID3v2::UniqueFileIdentifierFrame *)(*fl)[i];

    if(strcmp(ufif->owner().toCString(true), description) == 0
        || description[0] == '*') {
      oss.setf(std::ios::hex);

      for(TagLib::uint i = 0; i < ufif->identifier().size(); ++i) {
        if(isBinary) {
          sprintf(tmp, "%x", ufif->identifier()[i]);
          oss << tmp;
        } else {
          oss << ufif->identifier()[i];
        }
      }

      (*mSM_values)[field] = oss.str().c_str();

      return;
    }
  }
}



void ID3v2Reader::getPopmFrame(const char *field,
                               const char *description,
                               ID3v2::FrameList *fl)
{
  char tmp[2];
  ID3v2::UnknownFrame *uf;

  for(TagLib::uint i = 0; i < fl->size(); ++i) {
    uf = (ID3v2::UnknownFrame *)(*fl)[i];

    if(strcmp(uf->data().data(), description) == 0
        || description[0] == '*') {

      TagLib::uint j = 0;
      while(uf->data()[j] != 0) {
        if(++j >= uf->data().size()){
           return;
        }
      }

      sprintf(tmp, "%d", uf->data()[j + 1]);
      (*mSM_values)[field] = tmp;

      return;
    }
  }
}
