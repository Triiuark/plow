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
