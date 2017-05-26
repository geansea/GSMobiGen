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
    virtual void SetTitle(const char * pTitle);
    void SetType(GS_MOBI_TYPE type);
    void AddExthInfo(GS_MOBI_EXTH_TYPE type, const char * pValue);
    virtual void SetCover(const char * pCoverPath);
    virtual bool WriteTo(const char * pFilePath);

private:
    void AddExthInfo(GS_MOBI_EXTH_TYPE type, int value);

private:
    GSPalmDocHeader      m_palmHeader;
    GSMobiHeader         m_mobiHeader;
    vector<GSExthRecord> m_exthRecords;
    string               m_title;
    string               m_coverPath;
};

#endif /* GSMobiPacker_h */
