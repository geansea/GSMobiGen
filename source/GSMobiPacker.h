#ifndef GSMobiPacker_h
#define GSMobiPacker_h

#include "../include/IGSMobiPacker.h"
#include "GSMobiHelper.h"

class GSMobiPacker : public IGSMobiPacker
{
public:
    GSMobiPacker();
    virtual ~GSMobiPacker();

public:
    virtual bool WriteTo(const char * pFileName);
};

#endif /* GSMobiPacker_h */
