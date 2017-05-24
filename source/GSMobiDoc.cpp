#include "GSMobiDoc.h"

IGSMobiDoc * IGSMobiDoc::Create()
{
    return new GSMobiDoc();
}

GSMobiDoc::GSMobiDoc()
{
}

GSMobiDoc::~GSMobiDoc()
{
}

bool GSMobiDoc::Open(const string & fileName)
{
    if (!m_pdbDoc.Open(fileName))
    {
        return false;
    }
    return true;
}
