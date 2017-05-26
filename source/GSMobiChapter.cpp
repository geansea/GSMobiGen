#include "GSMobiChapter.h"

GSMobiChapter::GSMobiChapter()
{
}

GSMobiChapter::~GSMobiChapter()
{
}

void GSMobiChapter::SetTitle(const string & title)
{
    m_title = title;
}

void GSMobiChapter::SetContent(const string & content)
{
    m_content = content;
}

void GSMobiChapter::AddSubChapter(const GSMobiChapter & subChapter)
{
    m_subChapters.push_back(subChapter);
}
