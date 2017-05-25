#include "GSMobiHelper.h"

GSPalmDocHeader::GSPalmDocHeader()
    : compression(1)
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
{
}

void GSMobiHeader::ReadFrom(const char * p)
{
}

void GSMobiHeader::WriteTo(GSBytes & bytes) const
{
}
