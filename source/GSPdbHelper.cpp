#include "GSPdbHelper.h"
#include <time.h>

GSPdbHeader::GSPdbHeader()
    : name()
    , attributes(0)
    , version(0)
    , creationDate((uint32_t)time(NULL))
    , modificationDate(creationDate)
    , lastBackupDate(0)
    , modificationNumber(0)
    , appInfoOff(0)
    , sortInfoOff(0)
    , type('BOOK')
    , creator('MOBI')
    , uniqueIDSeed(0)
    , nextRecordListOff(0)
    , numRecords(0)
{
    memset(name, 0, sizeof(name));
}

void GSPdbHeader::ReadFrom(const char * p)
{
    GSGetArray(p, name, sizeof(name));
    GSGetU16BE(p, attributes);
    GSGetU16BE(p, version);
    GSGetU32BE(p, creationDate);
    GSGetU32BE(p, modificationDate);
    GSGetU32BE(p, lastBackupDate);
    GSGetU32BE(p, modificationNumber);
    GSGetU32BE(p, appInfoOff);
    GSGetU32BE(p, sortInfoOff);
    GSGetU32BE(p, type);
    GSGetU32BE(p, creator);
    GSGetU32BE(p, uniqueIDSeed);
    GSGetU32BE(p, nextRecordListOff);
    GSGetU16BE(p, numRecords);
}

void GSPdbHeader::WriteTo(GSBytes & bytes) const
{
    GSPushArray(bytes, name, sizeof(name));
    GSPushU16BE(bytes, attributes);
    GSPushU16BE(bytes, version);
    GSPushU32BE(bytes, creationDate);
    GSPushU32BE(bytes, modificationDate);
    GSPushU32BE(bytes, lastBackupDate);
    GSPushU32BE(bytes, modificationNumber);
    GSPushU32BE(bytes, appInfoOff);
    GSPushU32BE(bytes, sortInfoOff);
    GSPushU32BE(bytes, type);
    GSPushU32BE(bytes, creator);
    GSPushU32BE(bytes, uniqueIDSeed);
    GSPushU32BE(bytes, nextRecordListOff);
    GSPushU16BE(bytes, numRecords);
}

GSPdbRecord::GSPdbRecord()
    : dataOff(0)
    , attributes(0)
    , uniqueID(0)
{
}

void GSPdbRecord::ReadFrom(const char * p)
{
    GSGetU32BE(p, dataOff);
    GSGetU32BE(p, uniqueID);
    attributes = (uniqueID >> 24) & 0xFF;
    uniqueID &= 0x00FFFFFF;
}

void GSPdbRecord::WriteTo(GSBytes & bytes) const
{
    GSPushU32BE(bytes, dataOff);
    GSPushU32BE(bytes, (attributes << 24) | uniqueID);
}
