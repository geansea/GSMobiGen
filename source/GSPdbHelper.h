#ifndef GSPdbHelper_h
#define GSPdbHelper_h

#include <vector>

using namespace std;

typedef vector<char> GSBytes;

inline void GSGetArray(const char *& p, char * dst, size_t size)
{
    memcpy(dst, p, size);
    p += size;
}

inline void GSGetU16BE(const char *& p, uint16_t & u) {
    u = ((uint16_t)p[0] << 8) 
      + ((uint16_t)p[1]     );
    p += 2;
}

inline void GSGetU32BE(const char *& p, uint32_t & u) {
    u = ((uint32_t)p[0] << 24) 
      + ((uint32_t)p[1] << 16) 
      + ((uint32_t)p[2] <<  8) 
      + ((uint32_t)p[3]      );
    p += 4;
}

inline void GSPushArray(GSBytes & bytes, const char * p, size_t size)
{
#if 0
    bytes.insert(bytes.end(), p, p + size);
#else
    for (int i = 0; i < size; ++i)
    {
        bytes.push_back(p[i]);
    }
#endif
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
    uint32_t type;               // [3Ch~40h]: 'BOOK'
    uint32_t creator;            // [40h~44h]: 'MOBI'
    uint32_t uniqueIDSeed;       // [44h~48h]: 
    uint32_t nextRecordListOff;  // [48h~4Ch]: 
    uint16_t numRecords;         // [4Ch~4Eh]: 
    // Record List
    // Add 2 bytes padding

    // Default values for MOBI
    GSPdbHeader();
    void ReadFrom(const char *& p);
    void WriteTo(GSBytes & bytes) const;
};

#define GS_PDB_HEADER_LEN 0x4E

struct GSPdbRecord
{
    uint32_t dataOff;
    uint8_t  attributes;
    uint32_t uniqueID;

    GSPdbRecord();
    void ReadFrom(const char *& p);
    void WriteTo(GSBytes & bytes) const;
};

#define GS_PDB_RECORD_LEN 8

#endif /* GSPdbHelper_h */
