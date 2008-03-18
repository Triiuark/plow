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

std::map<std::string, std::string> ID3v2Reader::sMapping;

bool ID3v2Reader::sMappingDone = false;

ID3v2Reader::ID3v2Reader(const char * const fname)
    : AbstractReader(fname)
{
  initMapping();
}



void ID3v2Reader::initMapping() const
{
    if(sMapping.empty())
    {
      sMapping["comment"  ] = "COMM/";
      sMapping["id"       ] = "UFID/*";
      sMapping["language" ] = "TXXX/LANGUAGE";
      sMapping["lyrics"   ] = "TXXX/LYRICS";
      sMapping["mood"     ] = "TXXX/MOOD";
      sMapping["rating"   ] = "POPM/*";
      sMapping["release"  ] = "TXXX/RELEASE";
      sMapping["situation"] = "TXXX/SITUATION";
      sMapping["tempo"    ] = "TXXX/TEMPO";
    }
}



void ID3v2Reader::setMapping(StrMap &mapping) const
{
  if(!sMappingDone)
  {
    StrMapIt it = mapping.begin();
    while(it != mapping.end())
    {
      sMapping[it->first] = it->second;
      ++it;
    }
  }

  sMappingDone = true;
}



bool ID3v2Reader::mappingDone() const
{
  return sMappingDone;
}



void ID3v2Reader::read()
{
  ID3v2::Tag *tag;
  MPEG::File f(mValues["file"].c_str());

  if((tag = f.ID3v2Tag()))
  {
    mValues["artist"] = tag->artist().toCString(true);
    mValues["title" ] = tag->title().toCString(true);
    mValues["album" ] = tag->album().toCString(true);
    mValues["genre" ] = tag->genre().toCString(true);

    if(tag->year() > 0)
    {
      char tmp[24];
      sprintf(tmp, "%d", tag->year());
      mValues["date"] = tmp;
    }

    std::string s;
    ID3v2::TextIdentificationFrame *tif;
    unsigned int pos;

    if(!(tag->frameListMap()["TRCK"]).isEmpty())
    {
      tif = (ID3v2::TextIdentificationFrame *)tag->frameListMap()["TRCK"][0];
      s = tif->toString().toCString(true);
      pos = s.find('/', 0);
      if(pos == std::string::npos)
      {
        mValues["track"] = s;
      } else {
        mValues["track"]  = s.substr(0, pos);
        mValues["tracks"] = s.substr(++pos, s.size() - 1);
      }
    }

    if(!(tag->frameListMap()["TPOS"]).isEmpty())
    {
      tif = (ID3v2::TextIdentificationFrame *)tag->frameListMap()["TPOS"][0];
      s = tif->toString().toCString(true);
      pos = s.find('/', 0);
      if(pos == std::string::npos)
      {
        mValues["part"] = s;
      } else {
        mValues["part"]  = s.substr(0, pos);
        mValues["parts"] = s.substr(++pos, s.size() - 1);
      }
    }

    std::map<std::string, std::string>::iterator fieldsIt = sMapping.begin();
    const char *description;
    char frameName[5] = { 0, 0, 0, 0, 0 };
    while(fieldsIt != sMapping.end())
    {
      strncpy(frameName, fieldsIt->second.c_str(), 4);
      description = strchr(fieldsIt->second.c_str(), '/');

      if(description)
      {
        description = &description[1];
      } else {
        continue;
      }

      ID3v2::FrameList fl =
            (ID3v2::FrameList)(tag->frameListMap()[frameName]);
      if(!fl.isEmpty())
      {
        if(strcmp(frameName, "TXXX") == 0)
        {
          getTxxxFrame(fieldsIt->first.c_str(), description, &fl);
        }
        else if(strcmp(frameName, "COMM") == 0)
        {
          getCommFrame(fieldsIt->first.c_str(), description, &fl);
        }
        else if(strcmp(frameName, "UFID") == 0)
        {
          getUfidFrame(fieldsIt->first.c_str(), description, &fl);
        }
        else if(strcmp(frameName, "POPM") == 0)
        {
          getPopmFrame(fieldsIt->first.c_str(), description, &fl);
        }
      }
      ++fieldsIt;
    }

  }

  int len;
  char buff[16];
  if(f.audioProperties())
  {
    len = f.audioProperties()->length();
    sprintf(buff, "%d", len);
    mValues["length"] = (buff);
  }
}



void ID3v2Reader::getTxxxFrame(const char  * const field,
                               const char  * const description,
                               ID3v2::FrameList *fl)
{
  char *tmp;

  ID3v2::UserTextIdentificationFrame *utif;

  for(TagLib::uint i = 0; i < fl->size(); ++i)
  {
    utif = (ID3v2::UserTextIdentificationFrame *)(*fl)[i];

    if(strcmp(utif->description().toCString(true), description) == 0)
    {
      char *tmp2 = new char[strlen(utif->toString().toCString(true)) + 1];
      sprintf(tmp2, "%s", utif->toString().toCString(true));

      tmp = strchr(tmp2, ']');
      if(tmp && strlen(tmp) > 2)
      {
        mValues[field] = &tmp[2]; /// remove '[description]'
      } else {
        mValues[field] = tmp2;
      }
      delete[] tmp2;
      return;
    }
  }
}



void ID3v2Reader::getCommFrame(const char * const field,
                               const char * const description,
                               ID3v2::FrameList *fl)
{
  ID3v2::CommentsFrame *comf;

  for(TagLib::uint i = 0; i < fl->size(); ++i)
  {
    comf = (ID3v2::CommentsFrame *)(*fl)[i];

    if(strcmp(comf->description().toCString(true), description) == 0)
    {
      mValues[field] = comf->toString().toCString(true);
      return;
    }
  }
}



void ID3v2Reader::getUfidFrame(const char * const field,
                               const char * const description,
                               ID3v2::FrameList *fl)
{
  char tmp[2];
  bool isBinary = false;

  ID3v2::UniqueFileIdentifierFrame *ufif;

  for(TagLib::uint i = 0; i < fl->size(); ++i)
  {
    ufif = (ID3v2::UniqueFileIdentifierFrame *)(*fl)[i];

    if(strcmp(ufif->owner().toCString(true), description) == 0
        || description[0] == '*')
    {
      for(TagLib::uint i = 0; i < ufif->identifier().size(); ++i)
      {
        if(isBinary)
        {
          sprintf(tmp, "%x", ufif->identifier()[i]);
          mValues[field] += tmp;
        } else {
          mValues[field] += ufif->identifier()[i];
        }
      }

      return;
    }
  }
}



void ID3v2Reader::getPopmFrame(const char * const field,
                               const char * const description,
                               ID3v2::FrameList *fl)
{
  char tmp[2];
  TagLib::uint j;
  ID3v2::UnknownFrame *uf;

  for(TagLib::uint i = 0; i < fl->size(); ++i)
  {
    uf = (ID3v2::UnknownFrame *)(*fl)[i];

    if(strcmp(uf->data().data(), description) == 0
        || description[0] == '*')
    {
      j = 0;
      while(uf->data()[j] != 0)
      {
        if(++j >= uf->data().size())
        {
           return;
        }
      }

      sprintf(tmp, "%d", uf->data()[j + 1]);
      mValues[field] = tmp;

      return;
    }
  }
}
