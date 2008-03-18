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
     * @param fname file to read tag from
     */
    AbstractReader(const char * const fname);

    /**
     * @returns the name of the config file section (e.g vorbis or id3v2)
     */
    virtual const char * const sectionName() const = 0;

    /**
     * This function is used to do the mapping of config file keys
     * to meta data keys (e.g. id = TXXX/ID in config file overwrites
     * the default in ID3v2Reader where it is UFID/)
     *
     * @param mapping a string map which contains the keys
     *                of the config file
     */
    virtual void setMapping(StrMap &mapping) const = 0;

    /**
     * @returns true if the mapping was done for an object of a derived
     *          class (so that it has only to be done
     *          once for all instances)
     */
    virtual bool mappingDone() const = 0;

    /**
     * This function should do all the meta data reading. Call it after
     * the mapping stuff.
     */
    virtual void read() = 0;

    /**
     * @returns a unique id of the file, an empty string if not found
     */
    const char * const id();

    /**
     * @returns title, an empty string if not found
     */
    const char * const title();

    /**
     * @returns artist, an empty string if not found
     */
    const char * const artist();

    /**
     * @returns album, an empty string if not found
     */
    const char * const album();

    /**
     * @returns part number, an empty string if not found
     */
    const char * const part();

    /**
     * @returns total number of parts, an empty string if not found
     */
    const char * const parts();

    /**
     * @returns track number, an empty string if not found
     */
    const char * const track();

    /**
     * @returns total number of tracks, an empty string if not found
     */
    const char * const tracks();

    /**
     * @returns genre, an empty string if not found
     */
    const char * const genre();

    /**
     * @returns rating, an empty string if not found
     */
    const char * const rating();

    /**
     * @returns mood, an empty string if not found
     */
    const char * const mood();

    /**
     * @returns situation, an empty string if not found
     */
    const char * const situation();

    /**
     * @returns tempo, an empty string if not found
     */
    const char * const tempo();

    /**
     * @returns language, an empty string if not found
     */
    const char * const language();

    /**
     * @returns date, an empty string if not found
     */
    const char * const date();

    /**
     * @returns comment, an empty string if not found
     */
    const char * const comment();

    /**
     * @returns length of song in seconds
     */
    const char * const length();

    /**
     * @returns file name (if a ...Reader class doesn't reads meta data
     *                     from file which should be added to database,
     *                     e.g. if data is read from another db)
     */
    const char * const file();




    /**
     * @returns 0 if no error occured
     */
    virtual int error() const;



    /**
     * destroys AbstractReader
     */
    virtual ~AbstractReader();

  protected:
    int    mErr;
    StrMap mValues;
};

#endif
