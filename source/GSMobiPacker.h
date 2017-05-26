#ifndef GSMobiPacker_h
#define GSMobiPacker_h

#include "../include/IGSMobiPacker.h"
#include "GSMobiHelper.h"
#include "GSMobiChapter.h"

class GSMobiPacker : public IGSMobiPacker
{
public:
    GSMobiPacker();
    virtual ~GSMobiPacker();

public:
    virtual void SetTitle(const char * pTitle);
    void SetType(GS_MOBI_TYPE type);
    void AddExthInfo(GS_MOBI_EXTH_TYPE type, const char * pValue);
    virtual void SetCover(const char * pCoverPath);
    virtual bool WriteTo(const char * pFilePath);

private:
    void AddExthInfo(GS_MOBI_EXTH_INNER_TYPE type, int value);
    string BuildMainHtml();
    GSBytes BuildRecord0();

private:
    GSPalmDocHeader       m_palmHeader;
    GSMobiHeader          m_mobiHeader;
    vector<GSExthRecord>  m_exthRecords;
    string                m_title;
    string                m_coverPath;
    vector<GSMobiChapter> m_chapters;
    int                   m_coverIndex;
    int                   m_thumbIndex;
    int                   m_bodyOffset;
};

#endif /* GSMobiPacker_h */
