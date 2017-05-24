#ifndef IGSMobiDoc_h
#define IGSMobiDoc_h

#include "GSMobiDef.h"

class IGSMobiDoc
{
public:
    static IGSMobiDoc * Create();

public:
    virtual ~IGSMobiDoc() {}

public:
    virtual bool Open(const string & fileName) = 0;
};

#endif /* IGSMobiDoc_h */
