#include "AbstractReader.h"

AbstractReader::AbstractReader(const char *fname, CStrMap *fields)
{
  mi_err     = 0;
  mcs_fname  = fname;
  mSM_values = new StrMap;
};


const char *AbstractReader::getId()
{
  return (*mSM_values)["id"].c_str();
}

const char *AbstractReader::getTitle()
{
  return (*mSM_values)["title"].c_str();
}

const char *AbstractReader::getArtist()
{
  return (*mSM_values)["artist"].c_str();
}

const char *AbstractReader::getAlbum()
{
  return (*mSM_values)["album"].c_str();
}

const char *AbstractReader::getPart()
{
  return (*mSM_values)["part"].c_str();
}

const char *AbstractReader::getParts()
{
  return (*mSM_values)["parts"].c_str();
}

const char *AbstractReader::getTrack()
{
  return (*mSM_values)["track"].c_str();
}

const char *AbstractReader::getTracks()
{
  return (*mSM_values)["tracks"].c_str();
}

const char *AbstractReader::getGenre()
{
  return (*mSM_values)["genre"].c_str();
}

const char *AbstractReader::getRating()
{
  return (*mSM_values)["rating"].c_str();
}

const char *AbstractReader::getMood()
{
  return (*mSM_values)["mood"].c_str();
}

const char *AbstractReader::getSituation()
{
  return (*mSM_values)["situation"].c_str();
}

const char *AbstractReader::getTempo()
{
  return (*mSM_values)["tempo"].c_str();
}

const char *AbstractReader::getLanguage()
{
  return (*mSM_values)["language"].c_str();
}

const char *AbstractReader::getDate()
{
  return (*mSM_values)["date"].c_str();
}

const char *AbstractReader::getComment()
{
  return (*mSM_values)["comment"].c_str();
}

const char *AbstractReader::getLength()
{
  return (*mSM_values)["length"].c_str();
}



const char *AbstractReader::get(const char * field)
{
  return (*mSM_values)[field].c_str();
}



int AbstractReader::error()
{
  return mi_err;
}



AbstractReader::~AbstractReader()
{
  if(mSM_values) {
    delete mSM_values;
  }
};
