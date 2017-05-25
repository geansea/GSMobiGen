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

void GSPalmDocHeader::ReadFrom(const char * p)
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
    : identifier()
    , headerLength()
    , mobiType()
    , textEncoding()
    , uniqueID()
    , fileVersion()
    , ortographicIndex()
    , inflectionIndex()
    , indexNames()
    , indexKeys()
    , extraIndex0()
    , extraIndex1()
    , extraIndex2()
    , extraIndex3()
    , extraIndex4()
    , extraIndex5()
    , firstNonBookIndex()
    , fullNameOffset()
    , fullNameLength()
    , locale()
    , inputLanguage()
    , outputLanguage()
    , minVersion()
    , firstImageIndex()
    , huffmanRecordOffset()
    , huffmanRecordCount()
    , huffmanTableOffset()
    , huffmanTableLength()
    , exthFlags()
    , unknownBytes1()
    , drmOffset()
    , drmCount()
    , drmSize()
    , drmFlags()
    , unknownBytes2()
    , unknownBytes3()
    , extraDataFlags()
    , indxRecordOffset()
{
}

void GSMobiHeader::ReadFrom(const char * p)
{
}

void GSMobiHeader::WriteTo(GSBytes & bytes) const
{
}
