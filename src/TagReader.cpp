
#include "TagReader.h"

#include "global.h"
#include "OggReader.h"
#include "ID3Reader.h"

TagReader::TagReader(const char* fname) : t(0), type(UNKNOWN) {

  a_array_cs oggFieldNames;
  oggFieldNames["id"     ] = "ID";
  oggFieldNames["artist" ] = "ARTIST";
  oggFieldNames["title"  ] = "TITLE";
  oggFieldNames["album"  ] = "ALBUM";
  oggFieldNames["part"   ] = "PARTINSET";
  oggFieldNames["parts"  ] = "PARTS";
  oggFieldNames["track"  ] = "TRACKNUMBER";
  oggFieldNames["tracks" ] = "TRACKS";
  oggFieldNames["genre"  ] = "GENRE";
  oggFieldNames["rating" ] = "RATING";
  oggFieldNames["comment"] = "COMMENT";
  oggFieldNames["year"   ] = "YEAR";

  /* not used */
  a_array_cs ID3v2FieldNames;
  ID3v2FieldNames["id"     ] = "UFID/reba";
  ID3v2FieldNames["artist" ] = "TPE1";
  ID3v2FieldNames["title"  ] = "TIT2";
  ID3v2FieldNames["album"  ] = "TALB";
  ID3v2FieldNames["part"   ] = "TPOS[0]";
  ID3v2FieldNames["parts"  ] = "TPOS[]";
  ID3v2FieldNames["track"  ] = "TRACKNUMBER";
  ID3v2FieldNames["tracks" ] = "TRACKS";
  ID3v2FieldNames["genre"  ] = "GENRE";
  ID3v2FieldNames["year"   ] = "YEAR";
  ID3v2FieldNames["comment"] = "COMMENT";

  char head[5];
  head[4] = 0;
  FILE *f = fopen(fname, "r");
  fread(head, 1, 4, f);
  fclose(f);

  if(strcmp("OggS", head) == 0) {
    type = OGG_VORBIS;
    t = new OggReader(fname, oggFieldNames);
  } else if(strncmp("ID3", head, 3) == 0) {
    type = MPEG;
    t = new ID3Reader(fname, oggFieldNames);
  }
}
