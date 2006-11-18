
#include "ID3Reader.h"

#include <string>

#include <tag.h>
#include <audioproperties.h>
#include <mpegfile.h>
#include <id3v2tag.h>
#include <textidentificationframe.h>

#include "global.h"

using namespace TagLib;

ID3Reader::ID3Reader(const char* fname, a_array_cs &fields) {
    values = new a_array_s;
    ID3v2::Tag *tag;
    MPEG::File f(fname);
    if((tag = f.ID3v2Tag())) {

      (*values)["artist" ] = tag->artist ().to8Bit(true);
      (*values)["title"  ] = tag->title  ().to8Bit(true);
      (*values)["album"  ] = tag->album  ().to8Bit(true);
      (*values)["genre"  ] = tag->genre  ().to8Bit(true);
      (*values)["comment"] = tag->comment().to8Bit(true);

      if(tag->year() > 0) {
        (*values)["year"] = "" + tag->year();
      }

      std::string s;
      ID3v2::TextIdentificationFrame *tif;
      unsigned int pos;

      tif = (ID3v2::TextIdentificationFrame *)tag->frameListMap()["TRCK"][0];
      if(tif) {
        s = tif->toString().to8Bit(true);
        pos = s.find('/', 0);
        if(pos == std::string::npos) {
          (*values)["track"] = s;
        } else {
          (*values)["track"]  = s.substr(0, pos);
          (*values)["tracks"] = s.substr(++pos, s.size() - 1);
        }
      }

      tif = (ID3v2::TextIdentificationFrame *)tag->frameListMap()["TPOS"][0];
      if(tif) {
        s = tif->toString().to8Bit(true);
        pos = s.find('/', 0);
        if(pos == std::string::npos) {
          (*values)["part"] = s;
        } else {
          (*values)["part"]  = s.substr(0, pos);
          (*values)["parts"] = s.substr(++pos, s.size() - 1);
        }
      }
    }

    if(f.audioProperties()) {
      int len = f.audioProperties()->length() * 1000;
      char buff[16];
      sprintf(buff, "%d", len);
      //buff[15] = 0;
      (*values)["length"] = (buff);
  }
}

const char *ID3Reader::getId()      {
  return (*values)["id"].c_str();
}

const char *ID3Reader::getArtist()  {
  return (*values)["artist"].c_str();
}

const char *ID3Reader::getTitle() {
  return (*values)["title"].c_str();
}

const char *ID3Reader::getAlbum() {
  return (*values)["album"].c_str();
}

const char *ID3Reader::getPart() {
  return (*values)["part"].c_str();
}

const char *ID3Reader::getParts() {
  return (*values)["parts"].c_str();
}

const char *ID3Reader::getTrack() {
  return (*values)["track"].c_str();
}

const char *ID3Reader::getTracks() {
  return (*values)["tracks"].c_str();
}

const char *ID3Reader::getGenre() {
  return (*values)["genre"].c_str();
}

const char *ID3Reader::getYear() {
  return (*values)["year"].c_str();
}

const char *ID3Reader::getRating()   {
  return (*values)["rating"].c_str();
}

const char *ID3Reader::getComment() {
  return (*values)["comment"].c_str();
}

const char *ID3Reader::getLength() {
  return (*values)["length"].c_str();
}

const char *ID3Reader::get(const char *field) {
  return (*values)[field].c_str();
}
