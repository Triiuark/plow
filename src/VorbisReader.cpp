#include "VorbisReader.h"

#include <tag.h>
#include <vorbisfile.h>
#include <xiphcomment.h>
#include <audioproperties.h>

using namespace TagLib;

std::map<std::string, std::string> VorbisReader::sMapping;

bool VorbisReader::sMappingDone = false;

VorbisReader::VorbisReader(const char * const fname)
    : AbstractReader(fname)
{
  initMapping();
}

void VorbisReader::initMapping() const
{
  if(sMapping.empty())
  {
    sMapping["album"    ] = "ALBUM";
    sMapping["artist"   ] = "ARTIST";
    sMapping["comment"  ] = "COMMENT";
    sMapping["date"     ] = "DATE";
    sMapping["genre"    ] = "GENRE";
    sMapping["id"       ] = "ID";
    sMapping["language" ] = "LANGUAGE";
    sMapping["lyrics"   ] = "LYRICS";
    sMapping["mood"     ] = "MOOD";
    sMapping["part"     ] = "DISCNUMBER";
    sMapping["parts"    ] = "DISCS";
    sMapping["rating"   ] = "RATING";
    sMapping["release"  ] = "RELEASE";
    sMapping["situation"] = "SITUATION";
    sMapping["tempo"    ] = "TEMPO";
    sMapping["title"    ] = "TITLE";
    sMapping["track"    ] = "TRACKNUMBER";
    sMapping["tracks"   ] = "TRACKS";
  }
}



void VorbisReader::setMapping(StrMap &mapping) const
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



bool VorbisReader::mappingDone() const
{
  return sMappingDone;
}



void VorbisReader::read()
{
  Ogg::Vorbis::File f(mValues["file"].c_str());
  Ogg::XiphComment *tag;

  if((tag = f.tag()))
  {
    const Ogg::FieldListMap *flm = &tag->fieldListMap();

    std::map<std::string, std::string>::iterator it = sMapping.begin();

    const StringList *sl;

    while(it != sMapping.end())
    {

      sl = &(*flm)[sMapping[it->first]];

      if(!sl->isEmpty())
      {
        mValues[it->first.c_str()] = (*sl)[0].toCString(true);
      }

      ++it;
    }
  }
  else
  {
    mErr = 1;
  }

  int len;
  char buff[16];
  if(f.audioProperties())
  {
    len = f.audioProperties()->length();
    sprintf(buff, "%d", len);
    mValues["length"] = buff;
  }
}
