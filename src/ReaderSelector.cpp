#include "ReaderSelector.h"

#include "VorbisReader.h"
#include "ID3v2Reader.h"

ReaderSelector::ReaderSelector(const char * const fname)
{
  mType = UNKNOWN;

  const int readSize = 290; /// needed 27 + number of page_segments
                            /// (max. 255)
                            /// + 8 for 0x01 + "vorbis"
  char head[readSize];

  FILE *f = fopen(fname, "r");

  fread(head, sizeof(char), readSize - 1, f);
  fclose(f);
  head[readSize - 1] = 0;

  if(strncmp("OggS", head, 4) == 0)
  {
    unsigned int pos = (unsigned char)head[26] + 28;

    if(strncmp(&head[pos + 1],  "vorbis", 6) == 0) //&& head[pos] == 0x01
    {
      mType = OGG_VORBIS;
      mReader.reset(new VorbisReader(fname));
      ((VorbisReader *)mReader.get())->setType(OGG_VORBIS);
    }
    else if(strncmp(&head[pos + 1], "FLAC", 4) == 0) //&& head[pos] == 0x7f
    {
      mType = OGG_FLAC;
      mReader.reset(new VorbisReader(fname));
      ((VorbisReader *)mReader.get())->setType(OGG_FLAC);
    }
  }
  else if(strncmp("ID3", head, 3) == 0)
  {
    mType = MPEG_ID3V2;

    mReader.reset(new ID3v2Reader(fname));
  }
}



AbstractReader * const ReaderSelector::reader() const
{
  if(mType != UNKNOWN)
  {
    return mReader.get();
  }

  return 0;
}



ReaderSelector::FileType ReaderSelector::fileType() const
{
  return mType;
}
