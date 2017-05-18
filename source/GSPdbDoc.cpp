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
    /*
    do
    {
        if (DKR_OK != m_pStream->Read(bytes, PDB_RECORD_LIST_LEN))
        {
            break;
        }

        m_pRecordList->nextRecordListOff = GetU32BE(bytes);
        m_pRecordList->numRecords = GetU16BE(bytes + 4);

        entryNum = m_entryNum;
        m_entryNum += m_pRecordList->numRecords;
        if (DK_NULL == m_pRecordList->pRecords)
        {
            m_pRecordList->pRecords = DK_MALLOCZ_OBJ_N(PdbRecord, m_entryNum);
            if (DK_NULL == m_pRecordList->pRecords)
            {
                break;
            }
        }
        else
        {
            pRecord = (PdbRecord *)DK_REALLOC(m_pRecordList->pRecords, m_entryNum * sizeof(PdbRecord));
            if (DK_NULL == pRecord)
            {
                break;
            }
            memset(pRecord + entryNum, 0, m_pRecordList->numRecords * sizeof(PdbRecord));
            m_pRecordList->pRecords = pRecord;
        }
        // ∂¡»°Record¡–±Ì
        for (; entryNum < m_entryNum; entryNum++)
        {
            if (DKR_OK != m_pStream->Read(bytes, PDB_RECORD_LEN))
            {
                break;
            }

            m_pRecordList->pRecords[entryNum].recordDataOff = GetU32BE(bytes);
            m_pRecordList->pRecords[entryNum].recordAttributes = *(bytes + 4);
            memcpy(m_pRecordList->pRecords[entryNum].uniqueID, bytes + 5, 3);
        }
    } while (m_pRecordList->nextRecordListOff);
    */
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
