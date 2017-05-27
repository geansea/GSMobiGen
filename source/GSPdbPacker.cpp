#include "GSPdbPacker.h"
#include <fstream>

GSPdbPacker::GSPdbPacker()
{
}

GSPdbPacker::~GSPdbPacker()
{
}

void GSPdbPacker::SetDatabaseName(const string & name)
{
    memcpy(m_header.name, name.c_str(), min(sizeof(m_header.name), name.length()));
}

void GSPdbPacker::AddRecord(const GSBytes & record)
{
    m_records.push_back(record);
}

bool GSPdbPacker::WriteTo(const char * pFilePath)
{
    // Create file
    ofstream file(pFilePath, ios::binary);
    if (file.fail())
    {
        return false;
    }
    // Prepare
    uint16_t recordCount = (uint16_t)m_records.size();
    m_header.numRecords = recordCount;
    m_header.uniqueIDSeed = (uint32_t)recordCount * 2 - 1;
    GSPdbRecord record;
    record.dataOff = GS_PDB_HEADER_LEN + recordCount * GS_PDB_RECORD_LEN + 2;
    // Output
    GSBytes bytes;
    m_header.WriteTo(bytes);
    for (uint16_t i = 0; i < recordCount; ++i)
    {
        record.uniqueID = i * 2;
        record.WriteTo(bytes);
        record.dataOff += (uint32_t)m_records[i].size();
    }
    GSPushPadding(bytes);
    for (uint16_t i = 0; i < recordCount; ++i)
    {
        bytes.insert(bytes.end(), m_records[i].begin(), m_records[i].end());
    }
    // Write file
    file.write(&bytes[0], bytes.size());
    file.close();
    return true;
}
