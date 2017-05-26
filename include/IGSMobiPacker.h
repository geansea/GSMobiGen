#ifndef IGSMobiPacker_h
#define IGSMobiPacker_h

#include "GSMobiDef.h"

class IGSMobiPacker
{
public:
    static IGSMobiPacker * Create();

public:
    virtual ~IGSMobiPacker() {}

public:
    virtual void SetTitle(const char * pTitle) = 0;
    virtual void SetCover(const char * pCoverPath) = 0;
    virtual bool WriteTo(const char * pFilePath) = 0;
};

#endif /* IGSMobiPacker_h */
