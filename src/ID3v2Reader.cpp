#include "ID3v2Reader.h"

#include <tag.h>
#include <audioproperties.h>
#include <mpegfile.h>
#include <id3v2tag.h>
#include <commentsframe.h>
#include <textidentificationframe.h>
#include <uniquefileidentifierframe.h>
#include <unknownframe.h>
#include <tlist.h>
#include <tmap.h>

using namespace TagLib;

std::map<std::string, std::string> ID3v2Reader::sMapping;

bool ID3v2Reader::sMappingDone = false;

ID3v2Reader::ID3v2Reader(const char * const fname) :
	AbstractReader(fname)
{
	initMapping();
}

void ID3v2Reader::initMapping() const
{
	if(sMapping.empty()) {
		sMapping["album"] = "TALB";
		sMapping["artist"] = "TPE1";
		sMapping["comment"] = "COMM/";
		sMapping["date"] = "TDRC";
		sMapping["genre"] = "TCON";
		sMapping["id"] = "UFID/*";
		sMapping["language"] = "TLAN";
		sMapping["lyrics"] = "TXXX/LYRICS";
		sMapping["mood"] = "TXXX/MOOD";
		sMapping["rating"] = "POPM/*";
		sMapping["release"] = "TDRL";
		sMapping["situation"] = "TXXX/SITUATION";
		sMapping["tempo"] = "TXXX/TEMPO";
		sMapping["title"] = "TIT2";
	}
}

void ID3v2Reader::setMapping(StrMap &mapping) const
{
	if(!sMappingDone) {
		StrMapIt it = mapping.begin();
		while(it != mapping.end()) {
			sMapping[it->first] = it->second;
			++it;
		}
	}

	sMappingDone = true;
}

bool ID3v2Reader::mappingDone() const
{
	return sMappingDone;
}

void ID3v2Reader::read()
{
	MPEG::File f(mValues["file"].c_str());
	ID3v2::Tag *tag;

	if((tag = f.ID3v2Tag())) {
		///
		/// track and part parsing
		///

		std::string s;
		ID3v2::TextIdentificationFrame *tif;
		size_t pos;

		if(!(tag->frameListMap()["TRCK"]).isEmpty()) {
			tif
					= (ID3v2::TextIdentificationFrame *)tag->frameListMap()["TRCK"][0];
			s = tif->toString().toCString(true);
			pos = s.find('/', 0);
			if(pos == std::string::npos) {
				mValues["track"] = s;
			} else {
				mValues["track"] = s.substr(0, pos);
				mValues["tracks"] = s.substr(++pos, s.size() - 1);
			}
		}

		if(!(tag->frameListMap()["TPOS"]).isEmpty()) {
			tif
					= (ID3v2::TextIdentificationFrame *)tag->frameListMap()["TPOS"][0];
			s = tif->toString().toCString(true);
			pos = s.find('/', 0);
			if(pos == std::string::npos) {
				mValues["part"] = s;
			} else {
				mValues["part"] = s.substr(0, pos);
				mValues["parts"] = s.substr(++pos, s.size() - 1);
			}
		}

		///
		/// all the other fields
		///

		std::map<std::string, std::string>::iterator fieldsIt =
				sMapping.begin();

		const char *description;
		char frameName[5] = { 0, 0, 0, 0, 0 };

		while(fieldsIt != sMapping.end()) {
			strncpy(frameName, fieldsIt->second.c_str(), 4);

			ID3v2::FrameList fl =
					(ID3v2::FrameList)(tag->frameListMap()[frameName]);

			if(fl.isEmpty()) {
				++fieldsIt;
				continue;
			}
			/// check for TextIdentificationFrame
			if(frameName[0] == 'T' && strcmp(frameName, "TXXX") != 0) {
				/// just use the first value fl.front()
				mValues[fieldsIt->first.c_str()]
						= fl.front()->toString().toCString(true);
			} else {
				description = strchr(fieldsIt->second.c_str(), '/');

				if(description) {
					description = &description[1];
				} else {
					description = 0;
				}

				if(strcmp(frameName, "TXXX") == 0) {
					getTxxxFrame(fieldsIt->first.c_str(), description,
							fl);
				} else if(strcmp(frameName, "COMM") == 0) {
					getCommFrame(fieldsIt->first.c_str(), description,
							fl);
				} else if(strcmp(frameName, "UFID") == 0) {
					getUfidFrame(fieldsIt->first.c_str(), description,
							fl);
				} else if(strcmp(frameName, "POPM") == 0) {
					getPopmFrame(fieldsIt->first.c_str(), description,
							fl);
				}
			}
			++fieldsIt;
		}
	}

	int len;
	char buff[16];
	if(f.audioProperties()) {
		len = f.audioProperties()->length();
		sprintf(buff, "%d", len);
		mValues["length"] = (buff);
	}
}

void ID3v2Reader::getTxxxFrame(const char * const field,
		const char * const description, ID3v2::FrameList &fl)
{
	ID3v2::UserTextIdentificationFrame *utif;

	for(TagLib::uint i = 0; i < fl.size(); ++i) {
		utif = (ID3v2::UserTextIdentificationFrame *)fl[i];

		if(strcmp(utif->description().toCString(true), description)
				== 0) {
			/// TODO: is this always correct?
			mValues[field] = utif->fieldList().back().toCString(true);

			return;
		}
	}
}

void ID3v2Reader::getCommFrame(const char * const field,
		const char * const description, ID3v2::FrameList &fl)
{
	ID3v2::CommentsFrame *comf;

	for(TagLib::uint i = 0; i < fl.size(); ++i) {
		comf = (ID3v2::CommentsFrame *)fl[i];

		if(strcmp(comf->description().toCString(true), description)
				== 0) {
			mValues[field] = comf->toString().toCString(true);
			return;
		}
	}
}

void ID3v2Reader::getUfidFrame(const char * const field,
		const char * const description, ID3v2::FrameList &fl)
{
	char tmp[2];


	/// TODO: ID3v2 standard says up to 64 Bytes of BINARY data, but
	///       should I convert readable id's into hex strings?
	///       workaround is now: if UFID contains only [:alnum:], '-' or '_'
	///       it is not converted into a hex string

	bool isBinary = false;
	std::string binaryData;

	ID3v2::UniqueFileIdentifierFrame *ufif;

	for(TagLib::uint i = 0; i < fl.size(); ++i) {
		ufif = (ID3v2::UniqueFileIdentifierFrame *)fl[i];

		if(strcmp(ufif->owner().toCString(true), description) == 0
				|| description[0] == '*') {
			for(TagLib::uint i = 0; i < ufif->identifier().size(); ++i) {
				mValues[field] += ufif->identifier()[i];

				sprintf(tmp, "%x", ufif->identifier()[i]);
				binaryData += tmp;

				tmp[0] = ufif->identifier()[i];

				if(!isBinary && !(tmp[0] == '-' || tmp[0] == '_'
						|| isalnum(tmp[0]))) {
					isBinary = true;
				}
			}

			if(isBinary) {
				mValues[field] = binaryData;
			}

			return;
		}
	}
}

void ID3v2Reader::getPopmFrame(const char * const field,
		const char * const description, ID3v2::FrameList &fl)
{
	char tmp[2];
	TagLib::uint j;
	ID3v2::UnknownFrame *uf;

	for(TagLib::uint i = 0; i < fl.size(); ++i) {
		uf = (ID3v2::UnknownFrame *)fl[i];

		if(strcmp(uf->data().data(), description) == 0
				|| description[0] == '*') {
			j = 0;
			while(uf->data()[j] != 0) {
				if(++j >= uf->data().size()) {
					return;
				}
			}

			sprintf(tmp, "%d", uf->data()[j + 1]);
			mValues[field] = tmp;

			return;
		}
	}
}
