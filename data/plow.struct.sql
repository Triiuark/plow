CREATE TABLE tbl_artist (
  id_artist INTEGER NOT NULL PRIMARY KEY,
  artist    TEXT    NOT NULL UNIQUE
);
INSERT INTO "tbl_artist" VALUES(1, '');



CREATE TABLE tbl_rating (
  id_rating INTEGER NOT NULL PRIMARY KEY,
  rating    TEXT    NOT NULL UNIQUE
);
INSERT INTO "tbl_rating" VALUES(1, '');



CREATE TABLE tbl_genre (
  id_genre INTEGER NOT NULL PRIMARY KEY,
  genre    TEXT    NOT NULL UNIQUE
);
INSERT INTO "tbl_genre" VALUES(1, '');



CREATE TABLE tbl_language (
  id_language INTEGER NOT NULL PRIMARY KEY,
  language    TEXT    NOT NULL UNIQUE
);
INSERT INTO "tbl_language" VALUES(1, '');



CREATE TABLE tbl_mood (
  id_mood INTEGER NOT NULL PRIMARY KEY,
  mood    TEXT    NOT NULL UNIQUE
);
INSERT INTO "tbl_mood" VALUES(1, '');



CREATE TABLE tbl_music (
  id_music       INTEGER NOT NULL PRIMARY KEY,
  file_id        INTEGER NOT NULL UNIQUE,
  file           TEXT    NOT NULL UNIQUE,
  title          TEXT    NOT NULL DEFAULT '',
  _id_artist     INTEGER NOT NULL DEFAULT 1,
  _id_album      INTEGER NOT NULL DEFAULT 1,
  part           INTEGER NOT NULL DEFAULT 1,
  track          INTEGER NOT NULL DEFAULT 0,
  length         INTEGER NOT NULL DEFAULT 0,
  _id_language   INTEGER NOT NULL DEFAULT 1,
  _id_rating     INTEGER NOT NULL DEFAULT 1,
  _id_genre      INTEGER NOT NULL DEFAULT 1,
  _id_mood       INTEGER NOT NULL DEFAULT 1,
  _id_situation  INTEGER NOT NULL DEFAULT 1,
  _id_tempo      INTEGER NOT NULL DEFAULT 1,
  release        INTEGER NOT NULL DEFAULT 0,
  status         INTEGER NOT NULL DEFAULT 0,
  comments       INTEGER NOT NULL DEFAULT '',
  lyrics_type    TEXT    NOT NULL DEFAULT 'txt',
  lyrics         TEXT    NOT NULL DEFAULT ''
);



CREATE TABLE tbl_album (
  id_album        INTEGER NOT NULL PRIMARY KEY,
  album           TEXT    NOT NULL DEFAULT '',
  album_id_artist INTEGER NOT NULL DEFAULT 1,
  tracks          INTEGER NOT NULL DEFAULT 0,
  parts           INTEGER NOT NULL DEFAULT 1,
  album_release   INTEGER NOT NULL DEFAULT 0
);
INSERT INTO "tbl_album" VALUES(1, '', 1, 0, 1, 0);



CREATE TABLE tbl_situation (
   id_situation INTEGER NOT NULL PRIMARY KEY,
   situation    TEXT    NOT NULL UNIQUE
);
INSERT INTO "tbl_situation" VALUES(1, '');



CREATE TABLE tbl_tempo (
  id_tempo INTEGER NOT NULL PRIMARY KEY,
  tempo    TEXT    NOT NULL UNIQUE
);
INSERT INTO "tbl_tempo" VALUES(1, '');



CREATE TABLE tbl_tmp (
   tmp_id           INTEGER NOT NULL PRIMARY KEY,
   tmp_file_id      INTEGER NOT NULL DEFAULT 0,
   tmp_file         TEXT    NOT NULL DEFAULT '',
   tmp_title        TEXT    NOT NULL DEFAULT '',
   tmp_artist       TEXT    NOT NULL DEFAULT '',
   tmp_album        TEXT    NOT NULL DEFAULT '',
   tmp_part         INTEGER NOT NULL DEFAULT 1,
   tmp_parts        INTEGER NOT NULL DEFAULT 1,
   tmp_track        INTEGER NOT NULL DEFAULT 0,
   tmp_tracks       INTEGER NOT NULL DEFAULT 0,
   tmp_length       INTEGER NOT NULL DEFAULT 0,
   tmp_release      INTEGER NOT NULL DEFAULT 0,
   tmp_genre        TEXT    NOT NULL DEFAULT '',
   tmp_rating       TEXT    NOT NULL DEFAULT '',
   tmp_mood         TEXT    NOT NULL DEFAULT '',
   tmp_situation    TEXT    NOT NULL DEFAULT '',
   tmp_tempo        TEXT    NOT NULL DEFAULT '',
   tmp_language     TEXT    NOT NULL DEFAULT '',
   tmp_lyrics       TEXT    NOT NULL DEFAULT '',
   tmp_comments     TEXT    NOT NULL DEFAULT '',
   tmp_id_artist    INTEGER NOT NULL DEFAULT 1,
   tmp_id_album     INTEGER NOT NULL DEFAULT 1,
   tmp_id_genre     INTEGER NOT NULL DEFAULT 1,
   tmp_id_rating    INTEGER NOT NULL DEFAULT 1,
   tmp_id_mood      INTEGER NOT NULL DEFAULT 1,
   tmp_id_situation INTEGER NOT NULL DEFAULT 1,
   tmp_id_tempo     INTEGER NOT NULL DEFAULT 1,
   tmp_id_language  INTEGER NOT NULL DEFAULT 1
);
