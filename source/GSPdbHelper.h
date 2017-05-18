#ifndef GSPdbHelper_h
#define GSPdbHelper_h

#include "../include/GSMobiDef.h"

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

struct GSPdbRecord
{
    uint32_t recordDataOff;
    uint8_t  recordAttributes;
    uint8_t  uniqueID[3];

    GSPdbRecord()
    {
        memset(this, 0, sizeof(*this));
    }
};

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

#endif /* GSPdbHelper_h */
