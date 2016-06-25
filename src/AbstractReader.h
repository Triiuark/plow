/* vi:set fenc=utf-8 ts=4 sw=4: */

/***********************************************************************
 *                                                                     *
 *  Copyright (C) 2006 -2010  René Bählkow <reba@projekt182.de>        *
 *                                                                     *
 *  This file is part of Plow.                                         *
 *                                                                     *
 *  Plow is free software; you can redistribute it and/or              *
 *  modify it under the terms of the GNU General Public License        *
 *  as published by the Free Software Foundation; either version 2     *
 *  of the License, or (at your option) any later version.             *
 *                                                                     *
 *  Plow is distributed in the hope that it will be useful,            *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      *
 *  GNU General Public License for more details.                       *
 *                                                                     *
 *  You should have received a copy of the GNU General Public License  *
 *  along with Plow.  If not, see <http://www.gnu.org/licenses/>       *
 *                                                                     *
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
		 * @returns album, an empty string if not found
		 */
		const char * const album();

		/**
		 * @returns artist, an empty string if not found
		 */
		const char * const artist();

		/**
		 * @returns comment, an empty string if not found
		 */
		const char * const comment();

		/**
		 * @returns date, an empty string if not found
		 */
		const char * const date();

		/**
		 * @returns file name (if a ...Reader class doesn't reads meta data
		 *                     from file which should be added to database,
		 *                     e.g. if data is read from another db)
		 */
		const char * const file();

		/**
		 * @returns genre, an empty string if not found
		 */
		const char * const genre();

		/**
		 * @returns a unique id of the file, an empty string if not found
		 */
		const char * const id();

		/**
		 * @returns language, an empty string if not found
		 */
		const char * const language();

		/**
		 * @returns length of song in seconds
		 */
		const char * const length();

		/**
		 * @returns lyrics, an empty string if not found
		 */
		const char * const lyrics();

		/**
		 * @returns mood, an empty string if not found
		 */
		const char * const mood();

		/**
		 * @returns part number, an empty string if not found
		 */
		const char * const part();

		/**
		 * @returns total number of parts, an empty string if not found
		 */
		const char * const parts();

		/**
		 * @returns rating, an empty string if not found
		 */
		const char * const rating();

		/**
		 * @returns album release date, an empty string if not found
		 */
		const char * const release();

		/**
		 * @returns situation, an empty string if not found
		 */
		const char * const situation();

		/**
		 * @returns tempo, an empty string if not found
		 */
		const char * const tempo();

		/**
		 * @returns title, an empty string if not found
		 */
		const char * const title();

		/**
		 * @returns track number, an empty string if not found
		 */
		const char * const track();

		/**
		 * @returns total number of tracks, an empty string if not found
		 */
		const char * const tracks();

		/**
		 * @returns 0 if no error occured
		 */
		virtual int error() const;

		/**
		 * destroys AbstractReader
		 */
		virtual ~AbstractReader();

	protected:
		int mErr;
		StrMap mValues;
};

#endif
