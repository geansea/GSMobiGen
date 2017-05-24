#include "GSMobiPacker.h"

IGSMobiPacker * IGSMobiPacker::Create()
{
    return new GSMobiPacker();
}

GSMobiPacker::GSMobiPacker()
{
}

GSMobiPacker::~GSMobiPacker()
{
}

bool GSMobiPacker::WriteTo(const char * pFileName)
{
    return false;
}
