#include <stdlib.h>
#include <time.h>
#include "include/IGSMobiPacker.h"

int main(int argc, char* argv[])
{
    srand((unsigned int)time(NULL));
    IGSMobiPacker *packer = IGSMobiPacker::Create();
    packer->SetDatabaseName("Test Book");
    packer->SetTitle("Test");
    packer->SetType(GS_MOBI_NEWS_MAGAZINE);
    packer->AddExthInfo(GS_MOBI_EXTH_AUTHOR, "Author");
    packer->AddExthInfo(GS_MOBI_EXTH_DESCRIPTION, "Description");
    packer->SetCover("F:/Test/mobi/cover.jpg");
    packer->SetThumb("F:/Test/mobi/thumb.jpg");
    packer->SetMasthead("F:/Test/mobi/masthead.gif");
    packer->AddSection("Section 1");
    packer->AddHtmlChapter("Chapter 1", "Content 1");
    packer->AddHtmlChapter("Chapter 2", "Content 2");
    packer->AddSection("Section 2");
    packer->AddHtmlChapter("Chapter 3", "Content 3");
    packer->AddHtmlChapter("Chapter 4", "Content 4");
    packer->WriteTo("F:/Test/mobi/test.mobi");
    delete packer;
    return 0;
}
