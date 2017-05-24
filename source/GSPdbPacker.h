#ifndef GSPdbPacker_h
#define GSPdbPacker_h

#include "GSPdbHelper.h"

class GSPdbPacker
{
public:
    GSPdbPacker();
    virtual ~GSPdbPacker();

public:
    virtual bool WriteTo(const char * pFileName);
};

#endif /* GSPdbPacker_h */
