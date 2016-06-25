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

#ifndef PLOW_READER_SELECTOR_H
#define PLOW_READER_SELECTOR_H

#include <memory>

#include "types.h"
#include "AbstractReader.h"

/**
 * @brief this one chooses the reader type
 */
class ReaderSelector
{
	public:
		/**
		 * supported file/tag types
		 */
		enum FileType
		{
			UNKNOWN = -1, /// unsupported file
			OGG_VORBIS, /// Ogg/Vorbis file with Vorbis comments
			OGG_FLAC, /// Ogg/FLAC file with Vorbis comments
			MPEG_ID3V2
		/// MP3 file with ID3v2 tag
		};

		/**
		 * Creates a new ReaderSelector object
		 *
		 * @param fname file name
		 */
		ReaderSelector(const char * const fname);

		/**
		 * @returns the FileType of the file
		 */
		FileType fileType() const;

		/**
		 * @returns an AbstractReader object if fileType is not UNKNOWN,
		 *          0 otherwise
		 */
		AbstractReader * const reader() const;

	private:
		FileType mType;
		std::auto_ptr<AbstractReader> mReader;
};

#endif
