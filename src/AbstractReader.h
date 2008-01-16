/***********************************************************************
* Copyright (C) 2006 by René Bählkow                                   *
*                                                                      *
* This program is free software; you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published by *
* the Free Software Foundation; either version 2 of the License, or    *
* (at your option) any later version.                                  *
*                                                                      *
* This program is distributed in the hope that it will be useful,      *
* but WITHOUT ANY WARRANTY; without even the implied warranty of       *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU General Public License for more details.                         *
*                                                                      *
* You should have received a copy of the GNU General Public License    *
* along with this program; if not, write to the                        *
* Free Software Foundation, Inc.,                                      *
* 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.            *
***********************************************************************/

#ifndef PLOW_ABSTRACT_READER_H
#define PLOW_ABSTRACT_READER_H

#include "types.h"

/**
 * @brief interface for all ...Reader classes
 */
class AbstractReader
{
  public:

    /**
     * creates a new AbstractReader
     *
     * @param fname  file to read tag from
     * @param fields if given the values of mCSM_fields
     *               are overwritten by the values of @a fields
     */
    AbstractReader(const char *fname, CStrMap *fields = 0);



    /**
     * @returns a unique id of the file, an empty string if not found
     */
    virtual const char *getId();

    /**
     * @returns title, an empty string if not found
     */
    virtual const char *getTitle();

    /**
     * @returns artist, an empty string if not found
     */
    virtual const char *getArtist();

    /**
     * @returns album, an empty string if not found
     */
    virtual const char *getAlbum();

    /**
     * @returns part number, an empty string if not found
     */
    virtual const char *getPart();

    /**
     * @returns total number of parts, an empty string if not found
     */
    virtual const char *getParts();

    /**
     * @returns track number, an empty string if not found
     */
    virtual const char *getTrack();

    /**
     * @returns total number of tracks, an empty string if not found
     */
    virtual const char *getTracks();

    /**
     * @returns genre, an empty string if not found
     */
    virtual const char *getGenre();

    /**
     * @returns rating, an empty string if not found
     */
    virtual const char *getRating();

    /**
     * @returns mood, an empty string if not found
     */
    virtual const char *getMood();

    /**
     * @returns situation, an empty string if not found
     */
    virtual const char *getSituation();

    /**
     * @returns tempo, an empty string if not found
     */
    virtual const char *getTempo();

    /**
     * @returns language, an empty string if not found
     */
    virtual const char *getLanguage();

    /**
     * @returns date, an empty string if not found
     */
    virtual const char *getDate();

    /**
     * @returns comment, an empty string if not found
     */
    virtual const char *getComment();

    /**
     * @returns length of song in seconds
     */
    virtual const char *getLength();



    /**
     * @param field name of a field (e.g. "artist")
     *
     * @returns the value for @a field, an empty string if not found
     */
    virtual const char *get(const char * field);



    /**
     * @returns 0 if no error occured
     */
    virtual int error();



    /**
     * destroys AbstractReader
     */
    virtual ~AbstractReader();

  protected:
    int        mi_err;
    const char *mcs_fname;
    StrMap     *mSM_values;
    CStrMap    mCSM_fields;
};

#endif
