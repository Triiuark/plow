#include "TagReader.h"

#include "VorbisReader.h"
#include "ID3v2Reader.h"

TagReader::TagReader(const char *fname, map<int, CStrMap *> fieldNames)
{
  mAR_reader = 0;
  mi_type    = UNKNOWN;
  mi_err     = 0;

  char head[5];
  head[4] = 0;
  FILE *f = fopen(fname, "r");
  fread(head, 1, 4, f);
  fclose(f);

  if(strncmp("OggS", head, 4) == 0) {
    mi_type    = VORBIS;
    mAR_reader = new VorbisReader(fname,
                                  fieldNames[TagReader::VORBIS]);
  } else if(strncmp("ID3", head, 3) == 0) {
    mi_type    = ID3V2;
    mAR_reader = new ID3v2Reader(fname, fieldNames[TagReader::ID3V2]);
  } else {
    mi_err = 0;
  }
}



const char *TagReader::id() const
{
  if(mAR_reader) {
    return mAR_reader->getId();
  }
  return 0;
}

const char *TagReader::artist() const
{
  if(mAR_reader) {
    return mAR_reader->getArtist();
  }
  return 0;
}

const char *TagReader::title() const
{
  if(mAR_reader) {
    return mAR_reader->getTitle();
  }
  return 0;
}

const char *TagReader::album() const
{
  if(mAR_reader) {
    return mAR_reader->getAlbum();
  }
  return 0;
}

const char *TagReader::part() const
{
  if(mAR_reader) {
    return mAR_reader->getPart();
  }
  return 0;
}

const char *TagReader::parts() const
{
  if(mAR_reader) {
    return mAR_reader->getParts();
  }
  return 0;
}

const char *TagReader::track() const
{
  if(mAR_reader) {
    return mAR_reader->getTrack();
  }
  return 0;
}

const char *TagReader::tracks() const
{
  if(mAR_reader) {
    return mAR_reader->getTracks();
  }
  return 0;
}

const char *TagReader::genre() const
{
  if(mAR_reader) {
    return mAR_reader->getGenre();
  }
  return 0;
}

const char *TagReader::rating() const
{
  if(mAR_reader) {
    return mAR_reader->getRating();
  }
  return 0;
}

const char *TagReader::mood() const
{
  if(mAR_reader) {
    return mAR_reader->getMood();
  }
  return 0;
}

const char *TagReader::situation() const
{
  if(mAR_reader) {
    return mAR_reader->getSituation();
  }
  return 0;
}

const char *TagReader::tempo() const
{
  if(mAR_reader) {
    return mAR_reader->getTempo();
  }
  return 0;
}

const char *TagReader::language() const
{
  if(mAR_reader) {
    return mAR_reader->getLanguage();
  }
  return 0;
}

const char *TagReader::date() const
{
  if(mAR_reader) {
    return mAR_reader->getDate();
  }
  return 0;
}

const char *TagReader::comment() const
{
  if(mAR_reader) {
    return mAR_reader->getComment();
  }
  return 0;
}

const char *TagReader::length() const
{
  if(mAR_reader) {
    return mAR_reader->getLength();
  }
  return 0;
}



int TagReader::fileType() const
{
  return mi_type;
}



int TagReader::error() const
{
  if(mAR_reader) {
    return mAR_reader->error();
  }
  return mi_err;
}



TagReader::~TagReader()
{
  if(mAR_reader) {
    delete mAR_reader;
  }
}
