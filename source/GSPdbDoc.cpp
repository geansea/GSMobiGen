#include "GSPdbDoc.h"
#include <fstream>

GSPdbDoc::GSPdbDoc()
{
}

GSPdbDoc::~GSPdbDoc()
{
}

bool GSPdbDoc::Open(const string & fileName)
{
    GSBytes pdbBytes;
    if (!ReadFile(fileName, pdbBytes))
    {
        return false;
    }
    if (pdbBytes.size() < PDB_HEADER_LEN + PDB_RECORD_LIST_LEN)
    {
        return false;
    }
    const char *p = &pdbBytes[0];
    // Header
    memcpy(m_header.name, p, 0x20); p += 0x20;
    m_header.attributes         = GSGetU16BE(p);
    m_header.version            = GSGetU16BE(p);
    m_header.creationDate       = GSGetU32BE(p);
    m_header.modificationDate   = GSGetU32BE(p);
    m_header.lastBackupDate     = GSGetU32BE(p);
    m_header.modificationNumber = GSGetU32BE(p);
    m_header.appInfoOff         = GSGetU32BE(p);
    m_header.sortInfoOff        = GSGetU32BE(p);
    memcpy(m_header.type, p, 4); p += 4;
    memcpy(m_header.creator, p, 4); p += 4;
    m_header.uniqueIDSeed       = GSGetU32BE(p);
    // Record List
    m_recordList.nextRecordListOff = GSGetU32BE(p);
    m_recordList.numRecords = GSGetU16BE(p);
    if (0 != m_recordList.nextRecordListOff)
    {
        // No necessary to support
        return false;
    }
    if (0 == m_recordList.numRecords)
    {
        return false;
    }
    if (pdbBytes.size() < PDB_HEADER_LEN + PDB_RECORD_LIST_LEN + m_recordList.numRecords * PDB_RECORD_LEN)
    {
        return false;
    }
    for (uint16_t i = 0; i < m_recordList.numRecords; ++i)
    {
        GSPdbRecord record;
        record.recordDataOff = GSGetU32BE(p);
        record.recordAttributes = *(p++);
        memcpy(record.uniqueID, p, 3); p += 3;
    }
    return true;
}

int GSPdbDoc::RecordCount()
{
    return 0;
}

const GSBytes * GSPdbDoc::GetRecord(int index)
{
    return NULL;
}

void GSPdbDoc::SetHeader(const GSPdbHeader & header)
{
}

void GSPdbDoc::AddRecord(const GSBytes & record)
{
}

bool GSPdbDoc::WriteTo(const string & fileName)
{
    return false;
}

bool GSPdbDoc::ReadFile(const string & fileName, GSBytes & bytes)
{
    ifstream file(fileName, ios::binary);
    if (file.fail())
    {
        return false;
    }
    file.unsetf(ios::skipws);
    file.seekg(0, std::ios::end);
    bytes.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&bytes[0], bytes.size());
    return true;
}

bool GSPdbDoc::WriteFile(const string & fileName, const GSBytes & bytes)
{
    ofstream file(fileName, ios::binary);
    if (file.fail())
    {
        return false;
    }
    file.write(&bytes[0], bytes.size());
    return true;
}
