#include "AbstractReader.h"

AbstractReader::AbstractReader(const char *fname, CStrMap *fields)
{
  mi_err     = 0;
  mcs_fname  = fname;
  mSM_values = new StrMap;
};


const char *AbstractReader::getId() const
{
  return (*mSM_values)["id"].c_str();
}

const char *AbstractReader::getTitle() const
{
  //mi_err = 12;
  return (*mSM_values)["title"].c_str();
}

const char *AbstractReader::getArtist() const
{
  return (*mSM_values)["artist"].c_str();
}

const char *AbstractReader::getAlbum() const
{
  return (*mSM_values)["album"].c_str();
}

const char *AbstractReader::getPart() const
{
  return (*mSM_values)["part"].c_str();
}

const char *AbstractReader::getParts() const
{
  return (*mSM_values)["parts"].c_str();
}

const char *AbstractReader::getTrack() const
{
  return (*mSM_values)["track"].c_str();
}

const char *AbstractReader::getTracks() const
{
  return (*mSM_values)["tracks"].c_str();
}

const char *AbstractReader::getGenre() const
{
  return (*mSM_values)["genre"].c_str();
}

const char *AbstractReader::getRating() const
{
  return (*mSM_values)["rating"].c_str();
}

const char *AbstractReader::getMood() const
{
  return (*mSM_values)["mood"].c_str();
}

const char *AbstractReader::getSituation() const
{
  return (*mSM_values)["situation"].c_str();
}

const char *AbstractReader::getTempo() const
{
  return (*mSM_values)["tempo"].c_str();
}

const char *AbstractReader::getLanguage() const
{
  return (*mSM_values)["language"].c_str();
}

const char *AbstractReader::getDate() const
{
  return (*mSM_values)["date"].c_str();
}

const char *AbstractReader::getComment() const
{
  return (*mSM_values)["comment"].c_str();
}

const char *AbstractReader::getLength() const
{
  return (*mSM_values)["length"].c_str();
}



const char *AbstractReader::get(const char *field) const
{
  return (*mSM_values)[field].c_str();
}



int AbstractReader::error() const
{
  return mi_err;
}



AbstractReader::~AbstractReader()
{
  if(mSM_values) {
    delete mSM_values;
  }
};
