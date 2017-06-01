#ifndef GSMobiPacker_h
#define GSMobiPacker_h

#include "../include/IGSMobiPacker.h"
#include "GSMobiHelper.h"

class GSMobiPacker : public IGSMobiPacker
{
public:
    GSMobiPacker();
    virtual ~GSMobiPacker();

public:
    virtual void SetDatabaseName(const char * pName);
    virtual void SetTitle(const char * pTitle);
    virtual void SetType(GS_MOBI_TYPE type);
    virtual void AddExthInfo(GS_MOBI_EXTH_TYPE type, const char * pValue);
    virtual void SetCover(const char * pCoverPath);
    virtual void SetThumb(const char * pThumbPath);
    virtual void SetMasthead(const char * pMastheadPath);
    virtual void AddSection(const char * pTitle);
    virtual void AddHtmlChapter(const char * pTitle, const char * pContent);
    virtual void AddTextChapter(const char * pTitle, const char * pContent);
    virtual bool WriteTo(const char * pFilePath);

private:
    void AddExthInfo(GS_MOBI_EXTH_INNER_TYPE type, int value);
    string BuildMainHtml();
    vector<GSBytes> BuildTextRecords(const string & html);
    GSBytes Lz77Compress(const GSBytes & bytes);
    GSBytes BuildINDXInfo(bool secondary = false);
    GSBytes BuildINDXValue(bool secondary = false);
    GSBytes BuildCNCX();
    GSBytes BuildFLIS();
    GSBytes BuildFCIS();
    GSBytes BuildEOF();
    GSBytes BuildRecord0();

private:
    GSPalmDocHeader       m_palmHeader;
    GSMobiHeader          m_mobiHeader;
    vector<GSExthRecord>  m_exthRecords;
    string                m_dbName;
    string                m_title;
    string                m_coverPath;
    string                m_thumbPath;
    string                m_mastheadPath;
    vector<GSMobiSection> m_sections;
    int                   m_coverIndex;
    int                   m_thumbIndex;
    int                   m_mastheadIndex;
};

#endif /* GSMobiPacker_h */
