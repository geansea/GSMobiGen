#include "GSMobiPacker.h"

IGSMobiPacker * IGSMobiPacker::Create()
{
    return new GSMobiPacker();
}

GSMobiPacker::GSMobiPacker()
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
    GSExthRecord record;
    record.type = type;
    record.length = 12;
    GSPushArray(record.data, pValue, strlen(pValue));
    m_exthRecords.push_back(record);
}

void GSMobiPacker::SetCover(const char * pCoverPath)
{
}

bool GSMobiPacker::WriteTo(const char * pFileName)
{
    return false;
}

void GSMobiPacker::AddExthInfo(GS_MOBI_EXTH_TYPE type, int value)
{
    GSExthRecord record;
    record.type = type;
    record.length = 12;
    GSPushU32BE(record.data, value);
    m_exthRecords.push_back(record);
}
