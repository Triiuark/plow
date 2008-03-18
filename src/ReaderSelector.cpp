#include "ReaderSelector.h"

#include "VorbisReader.h"
#include "ID3v2Reader.h"

ReaderSelector::ReaderSelector(const char * const fname)
{
  mType = UNKNOWN;

  char head[5];

  FILE *f = fopen(fname, "r");

  fread(head, sizeof(char), 4, f);
  fclose(f);

  if(strncmp("OggS", head, 4) == 0)
  {
    mType = VORBIS;

    mReader.reset(new VorbisReader(fname));
  }
  else if(strncmp("ID3", head, 3) == 0)
  {
    mType = ID3V2;

    mReader.reset(new ID3v2Reader(fname));
  }
}



AbstractReader * const ReaderSelector::reader() const
{
  return mReader.get();
}



ReaderSelector::FileType ReaderSelector::fileType() const
{
  return mType;
}
