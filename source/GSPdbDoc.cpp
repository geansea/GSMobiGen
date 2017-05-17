#include "GSPdbDoc.h"

GSPdbDoc::GSPdbDoc()
{
}

GSPdbDoc::~GSPdbDoc()
{
}

bool GSPdbDoc::Open(const string &fileName)
{
    return false;
}

int GSPdbDoc::RecordCount()
{
    return 0;
}

const GSBytes * GSPdbDoc::GetRecord(int index)
{
    return NULL;
}

void GSPdbDoc::SetHeader(const GSPdbHeader & header)
{
}

void GSPdbDoc::AddRecord(const GSBytes & record)
{
}

bool GSPdbDoc::WriteTo(const string & fileName)
{
    return false;
}
