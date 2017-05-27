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
    virtual void SetDatabaseName(const char * pName) = 0;
    virtual void SetTitle(const char * pTitle) = 0;
    virtual void SetType(GS_MOBI_TYPE type) = 0;
    virtual void AddExthInfo(GS_MOBI_EXTH_TYPE type, const char * pValue) = 0;
    virtual void SetCover(const char * pCoverPath) = 0;
    virtual void SetThumb(const char * pThumbPath) = 0;
    virtual void AddSection(const char * pTitle) = 0;
    virtual void AddHtmlChapter(const char * pTitle, const char * pContent) = 0;
    virtual void AddTextChapter(const char * pTitle, const char * pContent) = 0;
    virtual bool WriteTo(const char * pFilePath) = 0;
};

#endif /* IGSMobiPacker_h */
