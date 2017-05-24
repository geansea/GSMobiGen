#ifndef GSPdbHelper_h
#define GSPdbHelper_h

#include <vector>

using namespace std;

typedef vector<char> GSBytes;

inline void GSGetArray(const char *& p, char * dst, int size)
{
    memcpy(dst, p, size);
    p += size;
}

inline uint16_t GSGetU16BE(const char *& p) {
    uint16_t u = ((uint16_t)p[0] << 8) 
               + ((uint16_t)p[1]     );
    p += 2;
    return u;
}

inline uint32_t GSGetU32BE(const char *& p) {
    uint32_t u = ((uint32_t)p[0] << 24) 
               + ((uint32_t)p[1] << 16) 
               + ((uint32_t)p[2] <<  8) 
               + ((uint32_t)p[3]      );
    p += 4;
    return u;
}

inline void GSPushArray(GSBytes & bytes, const char * p, int size)
{
    for (int i = 0; i < size; ++i)
    {
        bytes.push_back(p[i]);
    }
}

inline void GSPushU16BE(GSBytes & bytes, uint16_t u)
{
    bytes.push_back((char)(u >> 8));
    bytes.push_back((char)(u     ));
}

inline void GSPushU32BE(GSBytes & bytes, uint32_t u)
{
    bytes.push_back((char)(u >> 24));
    bytes.push_back((char)(u >> 16));
    bytes.push_back((char)(u >>  8));
    bytes.push_back((char)(u      ));
}

// Based on http://wiki.mobileread.com/wiki/PDB

struct GSPdbHeader
{
    char     name[32];           // [00h~20h]: 
    uint16_t attributes;         // [20h~22h]: 
    uint16_t version;            // [22h~24h]: 
    uint32_t creationDate;       // [24h~28h]: 
    uint32_t modificationDate;   // [28h~2Ch]: 
    uint32_t lastBackupDate;     // [2Ch~30h]: 
    uint32_t modificationNumber; // [30h~34h]: 
    uint32_t appInfoOff;         // [34h~38h]: 
    uint32_t sortInfoOff;        // [38h~3Ch]: 
    char     type[4];            // [3Ch~40h]: 
    char     creator[4];         // [40h~44h]: 
    uint32_t uniqueIDSeed;       // [44h~48h]: 

    GSPdbHeader()
    {
        memset(this, 0, sizeof(*this));
    }
};

#define PDB_HEADER_LEN 0x48

struct GSPdbRecord
{
    uint32_t recordDataOff;
    uint8_t  recordAttributes;
    uint32_t uniqueID;

    GSPdbRecord()
    {
        memset(this, 0, sizeof(*this));
    }
};

#define PDB_RECORD_LEN 8

struct GSPdbRecordList
{
    uint32_t            nextRecordListOff; // [00h~04h]: 
    uint16_t            numRecords;        // [04h~06h]: 
    vector<GSPdbRecord> records;           // [06h~??h]: 
    // Add 2 bytes padding

    GSPdbRecordList()
        : nextRecordListOff(0)
        , numRecords(0)
    {
    }
};

#define PDB_RECORD_LIST_LEN 8

#endif /* GSPdbHelper_h */
