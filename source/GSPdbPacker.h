#ifndef GSPdbPacker_h
#define GSPdbPacker_h

#include "GSPdbHelper.h"

class GSPdbPacker
{
public:
    GSPdbPacker();
    virtual ~GSPdbPacker();

public:
    virtual void AddRecord(const GSBytes & record);
    virtual bool WriteTo(const char * pFileName);

private:
    GSPdbHeader     m_header;
    vector<GSBytes> m_records;
};

#endif /* GSPdbPacker_h */
