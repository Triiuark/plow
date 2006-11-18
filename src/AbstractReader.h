
#ifndef PLOW_ABSTRACT_READER_H
#define PLOW_ABSTRACT_READER_H

class AbstractReader {
  public:
    virtual ~AbstractReader() {}

    virtual int error()               = 0;

    virtual const char *getId()       = 0;
    virtual const char *getArtist()   = 0;
    virtual const char *getTitle()    = 0;
    virtual const char *getAlbum()    = 0;
    virtual const char *getPart()     = 0;
    virtual const char *getParts()    = 0;
    virtual const char *getTrack()    = 0;
    virtual const char *getTracks()   = 0;
    virtual const char *getGenre()    = 0;
    virtual const char *getRating()   = 0;
    virtual const char *getYear()     = 0;
    virtual const char *getComment()  = 0;
    virtual const char *getLength()   = 0;

    virtual const char *get(const char * field) = 0;
};

#endif
