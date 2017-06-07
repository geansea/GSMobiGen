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

void GSMobiPacker::SetMasthead(const char * pMastheadPath)
{
    m_mastheadPath = pMastheadPath;
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
    GSMobiChapter chapter;
    chapter.title = pTitle;
    chapter.SetPureTextContent(pContent);
    if (m_sections.empty())
    {
        AddSection("");
    }
    GSMobiSection &currSection = m_sections.back();
    currSection.chapters.push_back(chapter);
}

bool GSMobiPacker::WriteTo(const char * pFilePath)
{
    vector<GSBytes> pdbRecords(1, GSBytes());
    //
    // Text records
    //
    string html = BuildMainHtml();
    vector<GSBytes> textRecords = BuildTextRecords(html);
    m_palmHeader.textlength = (uint32_t)html.length();
    m_palmHeader.recordCount = (uint16_t)textRecords.size();
    size_t textRecordsLen = 0;
    for (size_t i = 0; i < textRecords.size(); ++i)
    {
        pdbRecords.push_back(textRecords[i]);
        textRecordsLen += textRecords[i].size();
    }
    if (textRecordsLen % 4)
    {
        GSBytes padding(4 - textRecordsLen % 4, 0);
        pdbRecords.push_back(padding);
    }
    m_mobiHeader.firstNonBookIndex = (uint32_t)pdbRecords.size();
    //
    // INDX & CNCX records
    //
    GSBytes cncxRecord;
    vector<GSMobiEntry> entries = BuildCNCX(cncxRecord);
    GSTagx tagx;
    pdbRecords.push_back(BuildINDXInfo(entries, tagx));
    pdbRecords.push_back(BuildINDXValue(entries, tagx));
    pdbRecords.push_back(cncxRecord);
    if (GS_MOBI_NEWS_MAGAZINE == m_mobiHeader.mobiType)
    {
        vector<GSMobiEntry> secondEntries;
        GSTagx secondTagx;
        pdbRecords.push_back(BuildINDXInfo(secondEntries, secondTagx, true));
        pdbRecords.push_back(BuildINDXValue(secondEntries, secondTagx, true));
    }
    //
    // Image records
    //
    m_mobiHeader.firstImageIndex = (uint32_t)pdbRecords.size();
    m_coverIndex = (int)(pdbRecords.size() - m_mobiHeader.firstImageIndex);

    m_thumbIndex = (int)(pdbRecords.size() - m_mobiHeader.firstImageIndex);

    //
    // FLIS & FCIS & End
    //
    m_mobiHeader.flisIndex = (uint32_t)pdbRecords.size();
    pdbRecords.push_back(BuildFLIS());
    m_mobiHeader.fcisIndex = (uint32_t)pdbRecords.size();
    pdbRecords.push_back(BuildFCIS());
    pdbRecords.push_back(BuildEOF());
    //
    // Record 0
    //
    pdbRecords[0] = BuildRecord0();
    //
    // Output
    //
    GSPdbPacker packer;
    packer.SetDatabaseName(m_dbName);
    for (size_t i = 0; i < pdbRecords.size(); ++i)
    {
        packer.AddRecord(pdbRecords[i]);
    }
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
    for (size_t i = 0; i < m_sections.size(); ++i)
    {
        GSMobiSection &section = m_sections[i];
        section.htmlBeginPos = html.length();
        for (size_t j = 0; j < section.chapters.size(); ++j)
        {
            GSMobiChapter &chapter = section.chapters[j];
            chapter.htmlBeginPos = html.length();
            snprintf(idString, POS_LEN, "%zu-%zu", i, j);
            html += "<h1 id=\"chap";
            html += idString;
            html += "\">" + chapter.title + "</h1>";
            html += "<div>" + chapter.content + "</div>";
            html += "<mbp:pagebreak />";
            chapter.htmlEndPos = html.length();
        }
        section.htmlEndPos = html.length();
    }
    html += "</body>";
    html += "</html>";
    // Fix pos
    snprintf(posString, POS_LEN, "%0*zu", POS_LEN, tocPos);
    html.replace(tocPosIndex, POS_LEN, posString, POS_LEN);
    snprintf(posString, POS_LEN, "%0*zu", POS_LEN, m_sections[0].htmlBeginPos);
    html.replace(startPosIndex, POS_LEN, posString, POS_LEN);
    return html;
}

vector<GSBytes> GSMobiPacker::BuildTextRecords(const string & html)
{
    vector<GSBytes> records;
    size_t pos = 0;
    size_t maxSize = m_palmHeader.recordSize;
    while (pos < html.length())
    {
        size_t size = min(html.length() - pos, maxSize);
        GSBytes record(html.begin() + pos, html.begin() + pos + size);
        pos += size;
        if (GS_MOBI_COMP_LZ77 == m_palmHeader.compression)
        {
            record = Lz77Compress(record);
        }
        char overlap = 0;
        for (; overlap < 3; ++overlap)
        {
            if (0x80 != (html[pos + overlap] & 0xC0))
            {
                break;
            }
            record.push_back(html[pos + overlap]);
        }
        record.push_back(overlap);
        records.push_back(record);
    }
    return records;
}

GSBytes GSMobiPacker::Lz77Compress(const GSBytes & bytes)
{
    // todo
    // fake compress
    GSBytes lz77;
    size_t pos = 0;
    while (pos < bytes.size())
    {
        size_t size = min(bytes.size() - pos, (size_t)8);
        lz77.push_back((char)size);
        lz77.insert(lz77.end(), bytes.begin() + pos, bytes.begin() + pos + size);
        pos += size;
    }
    return lz77;
}

vector<GSMobiEntry> GSMobiPacker::BuildEntries()
{
    vector<GSMobiEntry> entries;
    if (GS_MOBI_NEWS_MAGAZINE == m_mobiHeader.mobiType)
    {
        // periodical
        GSMobiEntry root;
        root.offset = m_sections.front().htmlBeginPos;
        root.length = m_sections.back().htmlEndPos - root.offset;
        root.label = TOC_STRING_EN;
        root.depth = 0;
        root.clazz = "periodical";
        root.child1 = 1;
        root.childN = m_sections.size();
        root.imageIndex = m_mastheadIndex;
        entries.push_back(root);
        // section
        int articleIndex = m_sections.size() + 1;
        for (size_t i = 0; i < m_sections.size(); ++i)
        {
            const GSMobiSection &section = m_sections[i];
            GSMobiEntry node;
            node.offset = section.htmlBeginPos;
            node.length = section.htmlEndPos - root.offset;
            node.label = section.title;
            node.depth = 1;
            node.clazz = "section";
            node.parent = 0;
            node.child1 = articleIndex;
            node.childN = articleIndex + section.chapters.size();
            entries.push_back(node);
            articleIndex += section.chapters.size();
        }
        // article
        for (size_t i = 0; i < m_sections.size(); ++i)
        {
            const GSMobiSection &section = m_sections[i];
            for (size_t j = 0; j < section.chapters.size(); ++j)
            {
                const GSMobiChapter &chapter = section.chapters[j];
                GSMobiEntry node;
                node.offset = section.htmlBeginPos;
                node.length = section.htmlEndPos - root.offset;
                node.label = section.title;
                node.depth = 2;
                node.clazz = "article";
                node.parent = i + 1;
                entries.push_back(node);
            }
        }
    }
    else
    {
    }
    return entries;
}

vector<GSMobiEntry> GSMobiPacker::BuildCNCX(GSBytes & cncx)
{
}

GSBytes GSMobiPacker::BuildINDXInfo(const vector<GSMobiEntry> & entries, const GSTagx & tagx, bool secondary)
{
    return GSBytes();
}

GSBytes GSMobiPacker::BuildINDXValue(const vector<GSMobiEntry> & entries, const GSTagx & tagx, bool secondary)
{
    return GSBytes();
}

GSBytes GSMobiPacker::BuildFLIS()
{
    GSBytes bytes;
    GSPushU32BE(bytes, 'FLIS');
    GSPushU32BE(bytes, 8);
    GSPushU16BE(bytes, 0x41);
    GSPushU16BE(bytes, 0);
    GSPushU32BE(bytes, 0);
    GSPushU32BE(bytes, 0xFFFFFFFF);
    GSPushU16BE(bytes, 1);
    GSPushU16BE(bytes, 3);
    GSPushU32BE(bytes, 3);
    GSPushU32BE(bytes, 1);
    GSPushU32BE(bytes, 0xFFFFFFFF);
    return bytes;
}

GSBytes GSMobiPacker::BuildFCIS()
{
    GSBytes bytes;
    GSPushU32BE(bytes, 'FCIS');
    GSPushU32BE(bytes, 0x14);
    GSPushU32BE(bytes, 0x10);
    GSPushU32BE(bytes, 1);
    GSPushU32BE(bytes, 0);
    GSPushU32BE(bytes, m_palmHeader.textlength);
    GSPushU32BE(bytes, 0);
    GSPushU32BE(bytes, 0x20);
    GSPushU32BE(bytes, 8);
    GSPushU16BE(bytes, 1);
    GSPushU16BE(bytes, 1);
    GSPushU32BE(bytes, 0);
    return bytes;
}

GSBytes GSMobiPacker::BuildEOF()
{
    GSBytes bytes;
    GSPushU32BE(bytes, '\xE9\x8E\x0D\x0A');
    return bytes;
}

GSBytes GSMobiPacker::BuildRecord0()
{
    // Prepare
    GSExthHeader exthHeader;
    AddExthInfo(GS_MOBI_EXTH_START_OFFSET, (int)m_sections[0].htmlBeginPos);
    AddExthInfo(GS_MOBI_EXTH_CREATOR_SOFTWARE, 202);
    AddExthInfo(GS_MOBI_EXTH_CREATOR_MAJOR_VER, 1);
    AddExthInfo(GS_MOBI_EXTH_CREATOR_MINOR_VER, 1);
    AddExthInfo(GS_MOBI_EXTH_CREATOR_BUILD_NUM, 99);
    AddExthInfo(GS_MOBI_EXTH_COVER_INDEX, m_coverIndex);
    AddExthInfo(GS_MOBI_EXTH_HAS_FAKE_COVER, 0);
    AddExthInfo(GS_MOBI_EXTH_THUMB_INDEX, m_thumbIndex);
    uint32_t exthHeaderLength = GS_EXTH_HEADER_LEN;
    for (uint32_t i = 0; i < exthHeader.recordCount; ++i)
    {
        exthHeaderLength += m_exthRecords[i].length;
    }
    exthHeader.headerLength = (exthHeaderLength + 3) / 4 * 4;
    m_mobiHeader.fullNameOffset = GS_PALM_DOC_HEADER_LEN + GS_MOBI_HEADER_LEN + exthHeader.headerLength;
    m_mobiHeader.fullNameLength = (uint32_t)m_title.length();
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
