
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
