
#ifndef PLOW_ID3_READER_H
#define PLOW_ID3_READER_H

#include "global.h"

#include "AbstractReader.h"

class ID3Reader : public AbstractReader {

  public:
    ID3Reader(
      const char* fname,
      a_array_cs &fields
    );

    ~ID3Reader() {}

    const char *getId();
    const char *getArtist();
    const char *getTitle();
    const char *getAlbum();
    const char *getPart();
    const char *getParts();
    const char *getTrack();
    const char *getTracks();
    const char *getGenre();
    const char *getYear();
    const char *getRating();
    const char *getComment();
    const char *getLength();

    const char *get(const char *field);

    int error() {return err;}

  private:
    int err;

    a_array_s *values;
};

#endif
