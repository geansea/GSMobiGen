#include "GSMobiPacker.h"
#include "GSPdbPacker.h"

IGSMobiPacker * IGSMobiPacker::Create()
{
    return new GSMobiPacker();
}

GSMobiPacker::GSMobiPacker()
    : m_coverIndex(0)
    , m_thumbIndex(0)
    , m_bodyOffset(0)
{
}

GSMobiPacker::~GSMobiPacker()
{
}

void GSMobiPacker::SetTitle(const char * pTitle)
{
}

void GSMobiPacker::SetType(GS_MOBI_TYPE type)
{
    m_mobiHeader.mobiType = type;
}

void GSMobiPacker::AddExthInfo(GS_MOBI_EXTH_TYPE type, const char * pValue)
{
    if (NULL == pValue || 0 == strlen(pValue))
    {
        return;
    }
    size_t valueLen = strlen(pValue);
    GSExthRecord record;
    record.type = type;
    record.length = 8 + valueLen;
    GSPushArray(record.data, pValue, valueLen);
    m_exthRecords.push_back(record);
}

void GSMobiPacker::SetCover(const char * pCoverPath)
{
}

bool GSMobiPacker::WriteTo(const char * pFilePath)
{
    // Main records
    // INDX records
    // Image records
    // Record 0
    GSBytes record0 = BuildRecord0();
    // 
    // Output
    GSPdbPacker packer;
    packer.AddRecord(record0);
    return packer.WriteTo(pFilePath);
}

void GSMobiPacker::AddExthInfo(GS_MOBI_EXTH_INNER_TYPE type, int value)
{
    GSExthRecord record;
    record.type = type;
    record.length = 12;
    GSPushU32BE(record.data, value);
    m_exthRecords.push_back(record);
}

string GSMobiPacker::BuildMainHtml()
{
    return string();
}

GSBytes GSMobiPacker::BuildRecord0()
{
    // Prepare
    GSExthHeader exthHeader;
    AddExthInfo(GS_MOBI_EXTH_START_OFFSET, m_bodyOffset);
    AddExthInfo(GS_MOBI_EXTH_CREATOR_SOFTWARE, 202);
    AddExthInfo(GS_MOBI_EXTH_CREATOR_MAJOR_VER, 1);
    AddExthInfo(GS_MOBI_EXTH_CREATOR_MINOR_VER, 1);
    AddExthInfo(GS_MOBI_EXTH_CREATOR_BUILD_NUM, 99);
    AddExthInfo(GS_MOBI_EXTH_COVER_INDEX, m_coverIndex);
    AddExthInfo(GS_MOBI_EXTH_HAS_FAKE_COVER, 0);
    AddExthInfo(GS_MOBI_EXTH_THUMB_INDEX, m_thumbIndex);
    // Output
    GSBytes bytes;
    m_palmHeader.WriteTo(bytes);
    m_mobiHeader.WriteTo(bytes);
    exthHeader.WriteTo(bytes);
    for (uint32_t i = 0; i < exthHeader.recordCount; ++i)
    {
        m_exthRecords[i].WriteTo(bytes);
    }
    GSPushPadding(bytes);
    GSPushArray(bytes, m_title.c_str(), m_title.length());
    GSPushU16BE(bytes, 0);
    GSPushPadding(bytes);
    // Add 0x0800 bytes space
    bytes.insert(bytes.end(), 0x0800, 0);
    return bytes;
}
