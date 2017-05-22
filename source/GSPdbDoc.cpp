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
        uint32_t uniqueID = GSGetU32BE(p);
        record.recordAttributes = (uniqueID >> 24) & 0xFF;
        record.uniqueID = uniqueID & 0x00FFFFFF;
        m_recordList.records.push_back(record);
    }
    // Record Datas
    for (uint16_t i = 0; i < m_recordList.numRecords; ++i)
    {
        uint32_t recordBeginOff = m_recordList.records[i].recordDataOff;
        uint32_t recordEndOff = pdbBytes.size();
        if (i + 1 < m_recordList.numRecords)
        {
            recordEndOff = m_recordList.records[i + 1].recordDataOff;
        }
        GSBytes recordData = GSBytes(pdbBytes.begin() + recordBeginOff, pdbBytes.begin() + recordEndOff);
        m_recordDatas.push_back(recordData);
    }
    return true;
}

int GSPdbDoc::RecordCount()
{
    return m_recordList.numRecords;
}

const GSBytes * GSPdbDoc::GetRecordData(int index)
{
    if (0 <= index && index < m_recordDatas.size())
    {
        return &m_recordDatas[index];
    }
    return NULL;
}

void GSPdbDoc::SetHeader(const GSPdbHeader & header)
{
    m_header = header;
}

void GSPdbDoc::AddRecordData(const GSBytes & recordData)
{
    m_recordDatas.push_back(recordData);
}

bool GSPdbDoc::WriteTo(const string & fileName)
{
    GSBytes pdbBytes;
    // Header
    GSPushArray(pdbBytes, m_header.name, 0x20);
    GSPushU16BE(pdbBytes, m_header.attributes);
    GSPushU16BE(pdbBytes, m_header.version);
    GSPushU32BE(pdbBytes, m_header.creationDate);
    GSPushU32BE(pdbBytes, m_header.modificationDate);
    GSPushU32BE(pdbBytes, m_header.lastBackupDate);
    GSPushU32BE(pdbBytes, m_header.modificationNumber);
    GSPushU32BE(pdbBytes, m_header.appInfoOff);
    GSPushU32BE(pdbBytes, m_header.sortInfoOff);
    GSPushArray(pdbBytes, m_header.type, 4);
    GSPushArray(pdbBytes, m_header.creator, 4);
    GSPushU32BE(pdbBytes, m_header.uniqueIDSeed);
    // Record List
    m_recordList.nextRecordListOff = 0;
    m_recordList.numRecords = m_recordDatas.size();
    GSPushU32BE(pdbBytes, m_recordList.nextRecordListOff);
    GSPushU16BE(pdbBytes, m_recordList.numRecords);
    uint32_t dataOffset = PDB_HEADER_LEN + PDB_RECORD_LIST_LEN + m_recordList.numRecords * PDB_RECORD_LEN;
    for (uint16_t i = 0; i < m_recordList.numRecords; ++i)
    {
        GSPdbRecord record;
        record.recordDataOff = dataOffset;
        record.recordAttributes = 0;
        record.uniqueID = i * 2;
        uint32_t uniqueID = (record.recordAttributes << 24) | record.uniqueID;
        GSPushU32BE(pdbBytes, record.recordDataOff);
        GSPushU32BE(pdbBytes, uniqueID);
        dataOffset += m_recordDatas[i].size();
    }
    GSPushU16BE(pdbBytes, 0); // padding
    // Record Datas
    for (uint16_t i = 0; i < m_recordList.numRecords; ++i)
    {
        pdbBytes.insert(pdbBytes.end(), m_recordDatas[i].begin(), m_recordDatas[i].end());
    }
    return WriteFile(fileName, pdbBytes);
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
