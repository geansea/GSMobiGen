#include "GSMobiHelper.h"

GSPalmDocHeader::GSPalmDocHeader()
    : compression(GS_MOBI_COMP_NONE)
    , unused(0)
    , textlength(0)
    , recordCount(0)
    , recordSize(0x1000)
    , encryptionType(0)
    , reserve(0)
{
}

void GSPalmDocHeader::ReadFrom(const char *& p)
{
    GSGetU16BE(p, compression);
    GSGetU16BE(p, unused);
    GSGetU32BE(p, textlength);
    GSGetU16BE(p, recordCount);
    GSGetU16BE(p, recordSize);
    GSGetU16BE(p, encryptionType);
    GSGetU16BE(p, reserve);
}

void GSPalmDocHeader::WriteTo(GSBytes & bytes) const
{
    GSPushU16BE(bytes, compression);
    GSPushU16BE(bytes, unused);
    GSPushU32BE(bytes, textlength);
    GSPushU16BE(bytes, recordCount);
    GSPushU16BE(bytes, recordSize);
    GSPushU16BE(bytes, encryptionType);
    GSPushU16BE(bytes, reserve);
}

GSMobiHeader::GSMobiHeader()
    : identifier('MOBI')
    , headerLength(GS_MOBI_HEADER_LEN)
    , mobiType(GS_MOBI_MOBIPOCKET_BOOK)
    , textEncoding(0xFDE9)
    , uniqueID(rand())
    , fileVersion(6)
    , ortographicIndex(0xFFFFFFFF)
    , inflectionIndex(0xFFFFFFFF)
    , indexNames(0xFFFFFFFF)
    , indexKeys(0xFFFFFFFF)
    , extraIndex0(0xFFFFFFFF)
    , extraIndex1(0xFFFFFFFF)
    , extraIndex2(0xFFFFFFFF)
    , extraIndex3(0xFFFFFFFF)
    , extraIndex4(0xFFFFFFFF)
    , extraIndex5(0xFFFFFFFF)
    , firstNonBookIndex(1)
    , fullNameOffset(0)
    , fullNameLength(0)
    , locale(GS_MOBI_LANGUAGE_EN)
    , inputLanguage(0)
    , outputLanguage(0)
    , minVersion(6)
    , firstImageIndex(0)
    , huffmanRecordOffset(0)
    , huffmanRecordCount(0)
    , huffmanTableOffset(0)
    , huffmanTableLength(0)
    , exthFlags(0x50)
    , unknown0(0)
    , unknown1(0)
    , unknown2(0)
    , unknown3(0)
    , unknown4(0)
    , unknown5(0)
    , unknown6(0)
    , unknown7(0)
    , drmOffset(0xFFFFFFFF)
    , drmCount(0xFFFFFFFF)
    , drmSize(0)
    , drmFlags(0)
    , unknown8(0)
    , unknown9(0)
    , unknown10(0)
    , firstContentIndex(1)
    , lastContentIndex(0)
    , unknown11(1)
    , fcisIndex(0)
    , fcisCount(1)
    , flisIndex(0)
    , flisCount(1)
    , unknown12(0)
    , unknown13(0)
    , srcsIndex(0xFFFFFFFF)
    , srcsCount(0)
    , unknown14(0xFFFFFFFF)
    , unknown15(0xFFFFFFFF)
    , extraDataFlags(0x01) // 0x03 or 0x0B
    , indxRecordOffset(0)
{
}

void GSMobiHeader::ReadFrom(const char *& p)
{
    GSGetU32BE(p, identifier);
    GSGetU32BE(p, headerLength);
    GSGetU32BE(p, mobiType);
    GSGetU32BE(p, textEncoding);
    GSGetU32BE(p, uniqueID);
    GSGetU32BE(p, fileVersion);
    GSGetU32BE(p, ortographicIndex);
    GSGetU32BE(p, inflectionIndex);
    GSGetU32BE(p, indexNames);
    GSGetU32BE(p, indexKeys);
    GSGetU32BE(p, extraIndex0);
    GSGetU32BE(p, extraIndex1);
    GSGetU32BE(p, extraIndex2);
    GSGetU32BE(p, extraIndex3);
    GSGetU32BE(p, extraIndex4);
    GSGetU32BE(p, extraIndex5);
    GSGetU32BE(p, firstNonBookIndex);
    GSGetU32BE(p, fullNameOffset);
    GSGetU32BE(p, fullNameLength);
    GSGetU32BE(p, locale);
    GSGetU32BE(p, inputLanguage);
    GSGetU32BE(p, outputLanguage);
    GSGetU32BE(p, minVersion);
    GSGetU32BE(p, firstImageIndex);
    GSGetU32BE(p, huffmanRecordOffset);
    GSGetU32BE(p, huffmanRecordCount);
    GSGetU32BE(p, huffmanTableOffset);
    GSGetU32BE(p, huffmanTableLength);
    GSGetU32BE(p, exthFlags);
    GSGetU32BE(p, unknown0);
    GSGetU32BE(p, unknown1);
    GSGetU32BE(p, unknown2);
    GSGetU32BE(p, unknown3);
    GSGetU32BE(p, unknown4);
    GSGetU32BE(p, unknown5);
    GSGetU32BE(p, unknown6);
    GSGetU32BE(p, unknown7);
    GSGetU32BE(p, drmOffset);
    GSGetU32BE(p, drmCount);
    GSGetU32BE(p, drmSize);
    GSGetU32BE(p, drmFlags);
    GSGetU32BE(p, unknown8);
    GSGetU32BE(p, unknown9);
    GSGetU32BE(p, unknown10);
    GSGetU16BE(p, firstContentIndex);
    GSGetU16BE(p, lastContentIndex);
    GSGetU32BE(p, unknown11);
    GSGetU32BE(p, fcisIndex);
    GSGetU32BE(p, fcisCount);
    GSGetU32BE(p, flisIndex);
    GSGetU32BE(p, flisCount);
    GSGetU32BE(p, unknown12);
    GSGetU32BE(p, unknown13);
    GSGetU32BE(p, srcsIndex);
    GSGetU32BE(p, srcsCount);
    GSGetU32BE(p, unknown14);
    GSGetU32BE(p, unknown15);
    GSGetU32BE(p, extraDataFlags);
    GSGetU32BE(p, indxRecordOffset);
}

void GSMobiHeader::WriteTo(GSBytes & bytes) const
{
    GSPushU32BE(bytes, identifier);
    GSPushU32BE(bytes, headerLength);
    GSPushU32BE(bytes, mobiType);
    GSPushU32BE(bytes, textEncoding);
    GSPushU32BE(bytes, uniqueID);
    GSPushU32BE(bytes, fileVersion);
    GSPushU32BE(bytes, ortographicIndex);
    GSPushU32BE(bytes, inflectionIndex);
    GSPushU32BE(bytes, indexNames);
    GSPushU32BE(bytes, indexKeys);
    GSPushU32BE(bytes, extraIndex0);
    GSPushU32BE(bytes, extraIndex1);
    GSPushU32BE(bytes, extraIndex2);
    GSPushU32BE(bytes, extraIndex3);
    GSPushU32BE(bytes, extraIndex4);
    GSPushU32BE(bytes, extraIndex5);
    GSPushU32BE(bytes, firstNonBookIndex);
    GSPushU32BE(bytes, fullNameOffset);
    GSPushU32BE(bytes, fullNameLength);
    GSPushU32BE(bytes, locale);
    GSPushU32BE(bytes, inputLanguage);
    GSPushU32BE(bytes, outputLanguage);
    GSPushU32BE(bytes, minVersion);
    GSPushU32BE(bytes, firstImageIndex);
    GSPushU32BE(bytes, huffmanRecordOffset);
    GSPushU32BE(bytes, huffmanRecordCount);
    GSPushU32BE(bytes, huffmanTableOffset);
    GSPushU32BE(bytes, huffmanTableLength);
    GSPushU32BE(bytes, exthFlags);
    GSPushU32BE(bytes, unknown0);
    GSPushU32BE(bytes, unknown1);
    GSPushU32BE(bytes, unknown2);
    GSPushU32BE(bytes, unknown3);
    GSPushU32BE(bytes, unknown4);
    GSPushU32BE(bytes, unknown5);
    GSPushU32BE(bytes, unknown6);
    GSPushU32BE(bytes, unknown7);
    GSPushU32BE(bytes, drmOffset);
    GSPushU32BE(bytes, drmCount);
    GSPushU32BE(bytes, drmSize);
    GSPushU32BE(bytes, drmFlags);
    GSPushU32BE(bytes, unknown8);
    GSPushU32BE(bytes, unknown9);
    GSPushU32BE(bytes, unknown10);
    GSPushU16BE(bytes, firstContentIndex);
    GSPushU16BE(bytes, lastContentIndex);
    GSPushU32BE(bytes, unknown11);
    GSPushU32BE(bytes, fcisIndex);
    GSPushU32BE(bytes, fcisCount);
    GSPushU32BE(bytes, flisIndex);
    GSPushU32BE(bytes, flisCount);
    GSPushU32BE(bytes, unknown12);
    GSPushU32BE(bytes, unknown13);
    GSPushU32BE(bytes, srcsIndex);
    GSPushU32BE(bytes, srcsCount);
    GSPushU32BE(bytes, unknown14);
    GSPushU32BE(bytes, unknown15);
    GSPushU32BE(bytes, extraDataFlags);
    GSPushU32BE(bytes, indxRecordOffset);
}

GSExthHeader::GSExthHeader()
    : identifier('EXTH')
    , headerLength(0)
    , recordCount(0)
{
}

void GSExthHeader::ReadFrom(const char *& p)
{
    GSGetU32BE(p, identifier);
    GSGetU32BE(p, headerLength);
    GSGetU32BE(p, recordCount);
}

void GSExthHeader::WriteTo(GSBytes & bytes) const
{
    GSPushU32BE(bytes, identifier);
    GSPushU32BE(bytes, headerLength);
    GSPushU32BE(bytes, recordCount);
}

GSExthRecord::GSExthRecord()
    : type(0)
    , length(0)
{
}

void GSExthRecord::ReadFrom(const char *& p)
{
    GSGetU32BE(p, type);
    GSGetU32BE(p, length);
    data.resize(length - 8, 0);
    GSGetArray(p, &data[0], data.size());
}

void GSExthRecord::WriteTo(GSBytes & bytes) const
{
    GSPushU32BE(bytes, type);
    GSPushU32BE(bytes, length);
    GSPushArray(bytes, &data[0], data.size());
}

GSIndxHeader::GSIndxHeader()
    : identifier('INDX')
    , headerLen(GS_INDX_HEADER_LEN)
    , unknown0(0)
    , unknown1(0)
    , indexType(0)
    , idxtOffset(0)
    , indexCount(1)
    , indexEncoding(0xFDE9)
    , indexLanguage(0xFFFFFFFF)
    , entryCount(0)
    , ordtOffset(0)
    , ligtOffset(0)
    , ligtCount(0)
    , cncxCount(1)
    , tagxOffset(0)
    , unknown33(0)
    , unknown34(0)
{
}

void GSIndxHeader::ReadFrom(const char *& p)
{
    GSGetU32BE(p, identifier);
    GSGetU32BE(p, headerLen);
    GSGetU32BE(p, unknown0);
    GSGetU32BE(p, unknown1);
    GSGetU32BE(p, indexType);
    GSGetU32BE(p, idxtOffset);
    GSGetU32BE(p, indexCount);
    GSGetU32BE(p, indexEncoding);
    GSGetU32BE(p, indexLanguage);
    GSGetU32BE(p, entryCount);
    GSGetU32BE(p, ordtOffset);
    GSGetU32BE(p, ligtOffset);
    GSGetU32BE(p, ligtCount);
    GSGetU32BE(p, cncxCount);
    p += 0x7C;
    GSGetU32BE(p, tagxOffset);
    GSGetU32BE(p, unknown33);
    GSGetU32BE(p, unknown34);
}

void GSIndxHeader::WriteTo(GSBytes & bytes) const
{
    GSPushU32BE(bytes, identifier);
    GSPushU32BE(bytes, headerLen);
    GSPushU32BE(bytes, unknown0);
    GSPushU32BE(bytes, unknown1);
    GSPushU32BE(bytes, indexType);
    GSPushU32BE(bytes, idxtOffset);
    GSPushU32BE(bytes, indexCount);
    GSPushU32BE(bytes, indexEncoding);
    GSPushU32BE(bytes, indexLanguage);
    GSPushU32BE(bytes, entryCount);
    GSPushU32BE(bytes, ordtOffset);
    GSPushU32BE(bytes, ligtOffset);
    GSPushU32BE(bytes, ligtCount);
    GSPushU32BE(bytes, cncxCount);
    bytes.insert(bytes.end(), 0x7C, 0);
    GSPushU32BE(bytes, tagxOffset);
    GSPushU32BE(bytes, unknown33);
    GSPushU32BE(bytes, unknown34);
}

GSTagx::GSTagx()
    : identifier('TAGX')
    , length(0)
    , controlByteCount(1)
{
}

void GSTagx::ReadFrom(const char *& p)
{
    GSGetU32BE(p, identifier);
    GSGetU32BE(p, length);
    GSGetU32BE(p, controlByteCount);
    for (uint16_t i = GS_TAGX_HEADER_LEN; i < length; i += 4)
    {
        uint32_t tag = 0;
        GSGetU32BE(p, tag);
        tags.push_back(tag);
    }
}

void GSTagx::WriteTo(GSBytes & bytes) const
{
    GSPushU32BE(bytes, identifier);
    GSPushU32BE(bytes, length);
    GSPushU32BE(bytes, controlByteCount);
    for (size_t i = 0; i < tags.size(); ++i)
    {
        GSPushU32BE(bytes, tags[i]);
    }
}

void GSTagx::AddTag(GS_TAGX_TAG tag, int valueNum, int mask)
{
    uint32_t tagValue = (tag << 24) | (valueNum << 16) | (mask << 8) | (GS_TAGX_END == tag);
    tags.push_back(tagValue);
}

GSMobiChapter::GSMobiChapter()
    : htmlBeginPos(0)
    , htmlEndPos(0)
{
}

void GSMobiChapter::SetPureTextContent(const string & text)
{
}

GSMobiSection::GSMobiSection()
    : htmlBeginPos(0)
    , htmlEndPos(0)
{
}
