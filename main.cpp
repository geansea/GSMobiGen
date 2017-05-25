#include "include/IGSMobiPacker.h"

int main(int argc, char* argv[])
{
    IGSMobiPacker *packer = IGSMobiPacker::Create();
    delete packer;
    return 0;
}
