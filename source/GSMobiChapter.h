#ifndef GSMobiChapter_h
#define GSMobiChapter_h

#include "GSMobiHelper.h"

class GSMobiChapter
{
public:
    GSMobiChapter();
    ~GSMobiChapter();

public:
    void SetTitle(const string & title);
    void SetContent(const string & content);
    void AddSubChapter(const GSMobiChapter & subChapter);

private:
    string                m_title;
    string                m_content;
    vector<GSMobiChapter> m_subChapters;
};

#endif /* GSMobiChapter_h */
