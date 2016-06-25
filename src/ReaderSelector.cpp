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

#include "ReaderSelector.h"

#include <cstdio>

#include "VorbisReader.h"
#include "ID3v2Reader.h"

ReaderSelector::ReaderSelector(const char * const fname)
{
	mType = UNKNOWN;

	const int readSize = 290; /// needed 27 + number of page_segments
	/// (max. 255)
	/// + 8 for 0x01 + "vorbis"
	char head[readSize];
	memset(head, 0, readSize);

	FILE *f = fopen(fname, "r");
	size_t len = fread(head, sizeof(char), readSize - 1, f);
	fclose(f);

	if(len != readSize - 1) {
		return;
	}

	if(strncmp("OggS", head, 4) == 0) {
		unsigned int pos = (unsigned char)head[26] + 27;


		//head[pos] == 0x01 &&
		if(strncmp(&head[pos + 1], "vorbis", 6) == 0) {
			mType = OGG_VORBIS;
			mReader.reset(new VorbisReader(fname));
			((VorbisReader *)mReader.get())->setType(OGG_VORBIS);
		}
		//head[pos] == 0x7f &&
		else if(strncmp(&head[pos + 1], "FLAC", 4) == 0) {
			mType = OGG_FLAC;
			mReader.reset(new VorbisReader(fname));
			((VorbisReader *)mReader.get())->setType(OGG_FLAC);
		}
	} else if(strncmp("ID3", head, 3) == 0) {
		mType = MPEG_ID3V2;

		mReader.reset(new ID3v2Reader(fname));
	}
}

AbstractReader * const ReaderSelector::reader() const
{
	if(mType != UNKNOWN) {
		return mReader.get();
	}

	return 0;
}

ReaderSelector::FileType ReaderSelector::fileType() const
{
	return mType;
}
