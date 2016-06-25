/* vi:set fenc=utf-8 ts=4 sw=4: */

/***********************************************************************
 *                                                                     *
 *  Copyright (C) 2006-2016 René Bählkow <triiuark@projekt182.de>      *
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

#ifndef PLOW_ID3V2_READER_H
#define PLOW_ID3V2_READER_H

#include <id3v2tag.h>

#include "AbstractReader.h"

/**
 * @see AbstractReader.h
 */
class ID3v2Reader: public AbstractReader
{
	public:
		ID3v2Reader(const char * const fname);

		const char * const sectionName() const
		{
			return "id3v2";
		}

		void setMapping(StrMap &mapping) const;

		bool mappingDone() const;

		void read();

	private:
		void initMapping() const;

		void getTxxxFrame(const char * const field,
				const char * const description,
				TagLib::ID3v2::FrameList &fl);

		void getCommFrame(const char * const field,
				const char * const description,
				TagLib::ID3v2::FrameList &fl);

		void getUfidFrame(const char * const field,
				const char * const description,
				TagLib::ID3v2::FrameList &fl);

		void getPopmFrame(const char * const field,
				const char * const description,
				TagLib::ID3v2::FrameList &fl);

		static bool sMappingDone;

		static std::map<std::string, std::string> sMapping;
};

#endif
