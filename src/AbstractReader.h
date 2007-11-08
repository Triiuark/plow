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

/**
 * @brief interface for all ...Reader classes
 */
class AbstractReader
{
  public:
    /**
     * @returns a unique id of the file, an empty string if not found
     */
    virtual const char *getId()        = 0;

    /**
     * @returns title, an empty string if not found
     */
    virtual const char *getTitle()     = 0;

    /**
     * @returns artist, an empty string if not found
     */
    virtual const char *getArtist()    = 0;

    /**
     * @returns album, an empty string if not found
     */
    virtual const char *getAlbum()     = 0;

    /**
     * @returns part number, an empty string if not found
     */
    virtual const char *getPart()      = 0;

    /**
     * @returns total number of parts, an empty string if not found
     */
    virtual const char *getParts()     = 0;

    /**
     * @returns track number, an empty string if not found
     */
    virtual const char *getTrack()     = 0;

    /**
     * @returns total number of tracks, an empty string if not found
     */
    virtual const char *getTracks()    = 0;

    /**
     * @returns genre, an empty string if not found
     */
    virtual const char *getGenre()     = 0;

    /**
     * @returns rating, an empty string if not found
     */
    virtual const char *getRating()    = 0;

    /**
     * @returns mood, an empty string if not found
     */
    virtual const char *getMood()      = 0;

    /**
     * @returns situation, an empty string if not found
     */
    virtual const char *getSituation() = 0;

    /**
     * @returns tempo, an empty string if not found
     */
    virtual const char *getTempo()     = 0;

    /**
     * @returns language, an empty string if not found
     */
    virtual const char *getLanguage()  = 0;

    /**
     * @returns date, an empty string if not found
     */
    virtual const char *getDate()      = 0;

    /**
     * @returns comment, an empty string if not found
     */
    virtual const char *getComment()   = 0;

    /**
     * @returns length of song in seconds
     */
    virtual const char *getLength()    = 0;



    /**
     * @param field name of a field (e.g. "artist")
     *
     * @returns the value for @a field, an empty string if not found
     */
    virtual const char *get(const char * field) = 0;



    /**
     * @returns 0 if no error occured
     */
    virtual int error() = 0;



    /**
     * destroys AbstractReader
     */
    virtual ~AbstractReader() {};
};

#endif
