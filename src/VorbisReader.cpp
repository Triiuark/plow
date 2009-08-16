#include "VorbisReader.h"

#include <memory>

#include <tag.h>
#include <oggfile.h>
#include <vorbisfile.h>
#include <oggflacfile.h>
#include <xiphcomment.h>
#include <audioproperties.h>

using namespace TagLib;

std::map<std::string, std::string> VorbisReader::sMapping;

bool VorbisReader::sMappingDone = false;

VorbisReader::VorbisReader(const char * const fname)
    : AbstractReader(fname)
{
  initMapping();

  mType = ReaderSelector::OGG_VORBIS;
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
  std::auto_ptr<Ogg::File> of;
  Ogg::XiphComment *tag;

  switch(mType)
  {
    case ReaderSelector::OGG_FLAC:
      of.reset(new Ogg::FLAC::File(mValues["file"].c_str()));
    break;

    default:
      of.reset(new Ogg::Vorbis::File(mValues["file"].c_str()));
    break;
  }

  tag = (Ogg::XiphComment *)of.get()->tag();

  if(tag)
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
  if(of->audioProperties())
  {
    len = of->audioProperties()->length();
    sprintf(buff, "%d", len);
    mValues["length"] = buff;
  }
}



void VorbisReader::setType(ReaderSelector::FileType type)
{
  mType = type;
}
