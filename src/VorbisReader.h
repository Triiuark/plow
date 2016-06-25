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

#ifndef PLOW_VORBIS_READER_H
#define PLOW_VORBIS_READER_H

#include "AbstractReader.h"
#include "ReaderSelector.h" /// for ReaderSelector::FileType
/**
 * @see AbstractReader.h
 */
class VorbisReader: public AbstractReader
{
	public:
		VorbisReader(const char * const fname);

		const char * const sectionName() const
		{
			return "vorbis";
		}

		void setMapping(StrMap &mapping) const;

		void setType(ReaderSelector::FileType);

		bool mappingDone() const;

		void read();

	private:
		void initMapping() const;

		static bool sMappingDone;
		static std::map<std::string, std::string> sMapping;

		ReaderSelector::FileType mType;
};

#endif
