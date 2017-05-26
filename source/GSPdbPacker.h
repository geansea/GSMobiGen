#ifndef GSPdbPacker_h
#define GSPdbPacker_h

#include "GSPdbHelper.h"

class GSPdbPacker
{
public:
    GSPdbPacker();
    ~GSPdbPacker();

public:
    void AddRecord(const GSBytes & record);
    bool WriteTo(const char * pFilePath);

private:
    GSPdbHeader     m_header;
    vector<GSBytes> m_records;
};

#endif /* GSPdbPacker_h */
