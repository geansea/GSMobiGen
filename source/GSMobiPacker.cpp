#include "GSMobiPacker.h"
#include "GSPdbPacker.h"

#define TOC_STRING_EN "Table of Contents"
#define TOC_STRING_ZH "\xE7\x9B\xAE\xE5\xBD\x95"
#define START_STRING_EN "Start"
#define START_STRING_ZH "\xE6\xAD\xA3\xE6\x96\x87"

IGSMobiPacker * IGSMobiPacker::Create()
{
    return new GSMobiPacker();
}

GSMobiPacker::GSMobiPacker()
    : m_coverIndex(0)
    , m_thumbIndex(0)
    , m_startOffset(0)
{
}

GSMobiPacker::~GSMobiPacker()
{
}

void GSMobiPacker::SetDatabaseName(const char * pName)
{
    m_dbName = pName;
}

void GSMobiPacker::SetTitle(const char * pTitle)
{
    m_title = pTitle;
}

void GSMobiPacker::SetType(GS_MOBI_TYPE type)
{
    m_mobiHeader.mobiType = type;
}

void GSMobiPacker::AddExthInfo(GS_MOBI_EXTH_TYPE type, const char * pValue)
{
    if (NULL == pValue || 0 == strlen(pValue))
    {
        return;
    }
    size_t valueLen = strlen(pValue);
    GSExthRecord record;
    record.type = type;
    record.length = 8 + (uint32_t)valueLen;
    GSPushArray(record.data, pValue, valueLen);
    m_exthRecords.push_back(record);
}

void GSMobiPacker::SetCover(const char * pCoverPath)
{
    m_coverPath = pCoverPath;
}

void GSMobiPacker::SetThumb(const char * pThumbPath)
{
    m_thumbPath = pThumbPath;
}

void GSMobiPacker::AddSection(const char * pTitle)
{
    GSMobiSection section;
    section.title = pTitle;
    m_sections.push_back(section);
}

void GSMobiPacker::AddHtmlChapter(const char * pTitle, const char * pContent)
{
    GSMobiChapter chapter;
    chapter.title = pTitle;
    chapter.content = pContent;
    if (m_sections.empty())
    {
        AddSection("");
    }
    GSMobiSection &currSection = m_sections.back();
    currSection.chapters.push_back(chapter);
}

void GSMobiPacker::AddTextChapter(const char * pTitle, const char * pContent)
{
}

bool GSMobiPacker::WriteTo(const char * pFilePath)
{
    // Main records
    // INDX records
    // Image records
    // Record 0
    GSBytes record0 = BuildRecord0();
    // 
    // Output
    GSPdbPacker packer;
    packer.SetDatabaseName(m_dbName);
    packer.AddRecord(record0);
    return packer.WriteTo(pFilePath);
}

void GSMobiPacker::AddExthInfo(GS_MOBI_EXTH_INNER_TYPE type, int value)
{
    GSExthRecord record;
    record.type = type;
    record.length = 12;
    GSPushU32BE(record.data, value);
    m_exthRecords.push_back(record);
}

string GSMobiPacker::BuildMainHtml()
{
    // Prepare
    string tocString = TOC_STRING_EN;
    string startString = START_STRING_EN;
    if (m_mobiHeader.locale == GS_MOBI_LANGUAGE_ZH)
    {
        tocString = TOC_STRING_ZH;
        startString = START_STRING_ZH;
    }
    const int POS_LEN = 10;
    char posString[POS_LEN + 1] = "0000000000";
    char idString[POS_LEN + 1] = "";
    // Output
    string html;
    html += "<html>";
    html += "<head>";
    html += "<guide>";
    html += "<reference title=\"" + tocString + "\" type=\"toc\" filepos=";
    size_t tocPosIndex = html.length();
    html += posString;
    html += "/>";
    html += "<reference title=\"" + startString + "\" type=\"text\" filepos=";
    size_t startPosIndex = html.length();
    html += posString;
    html += "/>";
    html += "</guide>";
    html += "</head>";
    html += "<body>";
    size_t tocPos = html.length();
    html += "<h1>" + tocString + "</h1>";
    for (size_t i = 0; i < m_sections.size(); ++i)
    {
        const GSMobiSection &section = m_sections[i];
        if (!section.title.empty())
        {
            html += "<h4>" + section.title + "</h4>";
        }
        html += "<ul>";
        for (size_t j = 0; j < section.chapters.size(); ++j)
        {
            const GSMobiChapter &chapter = section.chapters[j];
            snprintf(idString, POS_LEN, "%zu-%zu", i, j);
            html += "<li><a href=\"#chap";
            html += idString;
            html += "\">" + chapter.title + "</a></li>";
        }
        html += "</ul>";
    }
    html += "<mbp:pagebreak />";
    size_t startPos = html.length();
    for (size_t i = 0; i < m_sections.size(); ++i)
    {
        const GSMobiSection &section = m_sections[i];
        for (size_t j = 0; j < section.chapters.size(); ++j)
        {
            const GSMobiChapter &chapter = section.chapters[j];
            snprintf(idString, POS_LEN, "%zu-%zu", i, j);
            html += "<h1 id=\"chap";
            html += idString;
            html += "\">" + chapter.title + "</h1>";
            html += "<div>" + chapter.content + "</div>";
            html += "<mbp:pagebreak />";
        }
    }
    html += "</body>";
    html += "</html>";
    // Fix pos
    snprintf(posString, POS_LEN, "%0*zu", POS_LEN, tocPos);
    html.replace(tocPosIndex, POS_LEN, posString, POS_LEN);
    snprintf(posString, POS_LEN, "%0*zu", POS_LEN, startPos);
    html.replace(startPosIndex, POS_LEN, posString, POS_LEN);
    m_startOffset = (int)startPos;
    return html;
}

GSBytes GSMobiPacker::BuildRecord0()
{
    // Prepare
    GSExthHeader exthHeader;
    AddExthInfo(GS_MOBI_EXTH_START_OFFSET, m_startOffset);
    AddExthInfo(GS_MOBI_EXTH_CREATOR_SOFTWARE, 202);
    AddExthInfo(GS_MOBI_EXTH_CREATOR_MAJOR_VER, 1);
    AddExthInfo(GS_MOBI_EXTH_CREATOR_MINOR_VER, 1);
    AddExthInfo(GS_MOBI_EXTH_CREATOR_BUILD_NUM, 99);
    AddExthInfo(GS_MOBI_EXTH_COVER_INDEX, m_coverIndex);
    AddExthInfo(GS_MOBI_EXTH_HAS_FAKE_COVER, 0);
    AddExthInfo(GS_MOBI_EXTH_THUMB_INDEX, m_thumbIndex);
    // Output
    GSBytes bytes;
    m_palmHeader.WriteTo(bytes);
    m_mobiHeader.WriteTo(bytes);
    exthHeader.WriteTo(bytes);
    for (uint32_t i = 0; i < exthHeader.recordCount; ++i)
    {
        m_exthRecords[i].WriteTo(bytes);
    }
    GSPushPadding(bytes);
    GSPushArray(bytes, m_title.c_str(), m_title.length());
    GSPushU16BE(bytes, 0);
    GSPushPadding(bytes);
    // Add 0x0800 bytes space
    bytes.insert(bytes.end(), 0x0800, 0);
    return bytes;
}
