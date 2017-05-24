#ifndef GSMobiDoc_h
#define GSMobiDoc_h

#include "../include/IGSMobiDoc.h"
#include "GSPdbDoc.h"

class GSMobiDoc : public IGSMobiDoc
{
public:
    GSMobiDoc();
    virtual ~GSMobiDoc();

public:
    virtual bool Open(const string & fileName);

private:
    GSPdbDoc m_pdbDoc;
};

#endif /* GSMobiDoc_h */
