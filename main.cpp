#include <stdlib.h>
#include <time.h>
#include "include/IGSMobiPacker.h"

int main(int argc, char* argv[])
{
    srand((unsigned int)time(NULL));
    IGSMobiPacker *packer = IGSMobiPacker::Create();
    packer->SetTitle("");
    delete packer;
    return 0;
}
