#include "GSMobiPacker.h"
#include "GSPdbPacker.h"

#define TOC_STR_EN "Table of Contents"
#define TOC_STR_ZH "\xE7\x9B\xAE\xE5\xBD\x95"
#define START_STR_EN "Start"
#define START_STR_ZH "\xE6\xAD\xA3\xE6\x96\x87"

#define POS_STR_LEN 10

#include <stdarg.h>

string GSStringF(const char * format, ...)
{
    static char buffer[1024];
    va_list args;
    va_start(args, format);
    int n = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    return string(buffer, n);
}

IGSMobiPacker * IGSMobiPacker::Create()
{
    return new GSMobiPacker();
}

GSMobiPacker::GSMobiPacker()
    : m_coverIndex(0)
    , m_thumbIndex(1)
    , m_mastheadIndex(2)
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
    m_mobiHeader.indxRecordOffset = (uint32_t)pdbRecords.size();
    GSTagx tagx = BuildTagx();
    vector<GSMobiEntry> entries = BuildEntries();
    GSBytes cncx = BuildCNCX(entries);
    pdbRecords.push_back(BuildINDXInfo(entries, tagx));
    pdbRecords.push_back(BuildINDXValue(entries, tagx));
    pdbRecords.push_back(cncx);
    if (GS_MOBI_NEWS_MAGAZINE == m_mobiHeader.mobiType)
    {
        m_mobiHeader.indexNames = (uint32_t)pdbRecords.size();
        GSTagx secondTagx = BuildSecondTagx();
        vector<GSMobiEntry> secondEntries = BuildSecondEntries();
        pdbRecords.push_back(BuildINDXInfo(secondEntries, secondTagx));
        pdbRecords.push_back(BuildINDXValue(secondEntries, secondTagx));
    }
    //
    // Image records
    //
    // Cover
    m_mobiHeader.firstImageIndex = (uint32_t)pdbRecords.size();
    m_coverIndex = (int)(pdbRecords.size() - m_mobiHeader.firstImageIndex);
    GSBytes cover;
    GSReadFile(m_coverPath.c_str(), cover);
    GSPushPadding(cover);
    pdbRecords.push_back(cover);
    // Thumb
    m_thumbIndex = (int)(pdbRecords.size() - m_mobiHeader.firstImageIndex);
    GSBytes thumb;
    GSReadFile(m_thumbPath.c_str(), thumb);
    GSPushPadding(thumb);
    pdbRecords.push_back(thumb);
    // Masthead
    m_mastheadIndex = (int)(pdbRecords.size() - m_mobiHeader.firstImageIndex);
    GSBytes masthead;
    GSReadFile(m_mastheadPath.c_str(), masthead);
    GSPushPadding(masthead);
    pdbRecords.push_back(masthead);
    m_mobiHeader.lastContentIndex = (uint16_t)(pdbRecords.size() - 1);
    //
    // FLIS & FCIS & EOF
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
    string tocString = TOC_STR_EN;
    string startString = START_STR_EN;
    if (m_mobiHeader.locale == GS_MOBI_LANGUAGE_ZH)
    {
        tocString = TOC_STR_ZH;
        startString = START_STR_ZH;
    }
    string posStrHolder = GSStringF("%0*d", POS_STR_LEN, 0);
    vector<size_t> posIndexes;
    vector<size_t> positions;
    // Output
    string html;
    html += "<html>";
    html += "<head>";
    html += "<guide>";
    html += "<reference title=\"" + tocString + "\" type=\"toc\" filepos=";
    posIndexes.push_back(html.length());
    html += posStrHolder + " />";
    html += "<reference title=\"" + startString + "\" type=\"text\" filepos=";
    posIndexes.push_back(html.length());
    html += posStrHolder + " />";
    html += "</guide>";
    html += "</head>";
    html += "<body>";
    positions.push_back(html.length());
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
            html += "<li><a filepos=";
            posIndexes.push_back(html.length());
            html += posStrHolder + ">";
            html += chapter.title + "</a></li>";
        }
        html += "</ul>";
    }
    html += "<mbp:pagebreak />";
    positions.push_back(html.length());
    for (size_t i = 0; i < m_sections.size(); ++i)
    {
        GSMobiSection &section = m_sections[i];
        section.htmlBeginPos = html.length();
        for (size_t j = 0; j < section.chapters.size(); ++j)
        {
            positions.push_back(html.length());
            GSMobiChapter &chapter = section.chapters[j];
            chapter.htmlBeginPos = html.length();
            html += "<h1>" + chapter.title + "</h1>";
            html += "<div>" + chapter.content + "</div>";
            html += "<mbp:pagebreak />";
            chapter.htmlEndPos = html.length();
        }
        section.htmlEndPos = html.length();
    }
    html += "</body>";
    html += "</html>";
    // Update positions
    for (size_t i = 0; i < posIndexes.size(); ++i)
    {
        string posString = GSStringF("%0*zu", POS_STR_LEN, positions[i]);
        html.replace(posIndexes[i], POS_STR_LEN, posString);
    }
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
        record.push_back(overlap | 0x20);
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

GSTagx GSMobiPacker::BuildTagx()
{
    GSTagx tagx;
    tagx.AddTag(GS_TAGX_Offset,      1, 0x01);
    tagx.AddTag(GS_TAGX_Length,      1, 0x02);
    tagx.AddTag(GS_TAGX_LabelOffset, 1, 0x04);
    tagx.AddTag(GS_TAGX_Depth,       1, 0x08);
    tagx.AddTag(GS_TAGX_ClassOffset, 1, 0x10);
    tagx.AddTag(GS_TAGX_Parent,      1, 0x20);
    tagx.AddTag(GS_TAGX_Child1,      1, 0x40);
    tagx.AddTag(GS_TAGX_ChildN,      1, 0x80);
    tagx.AddTag(GS_TAGX_END,         0, 0x00);
    if (GS_MOBI_NEWS_MAGAZINE == m_mobiHeader.mobiType)
    {
        tagx.controlByteCount = 2;
        tagx.AddTag(GS_TAGX_ImageIndex,   1, 0x01);
        tagx.AddTag(GS_TAGX_DescOffset,   1, 0x02);
        tagx.AddTag(GS_TAGX_AuthorOffset, 1, 0x04);
        tagx.AddTag(GS_TAGX_END,          0, 0x00);
    }
    return tagx;
}

vector<GSMobiEntry> GSMobiPacker::BuildEntries()
{
    vector<GSMobiEntry> entries;
    if (GS_MOBI_NEWS_MAGAZINE == m_mobiHeader.mobiType)
    {
        // periodical
        GSMobiEntry root;
        root.offset = (int)m_sections.front().htmlBeginPos;
        root.length = (int)m_sections.back().htmlEndPos - root.offset;
        root.label = TOC_STR_EN;
        root.depth = 0;
        root.clazz = "periodical";
        root.child1 = 1;
        root.childN = (int)m_sections.size();
        root.imageIndex = m_mastheadIndex;
        entries.push_back(root);
        // section
        int articleIndex = (int)m_sections.size();
        for (size_t i = 0; i < m_sections.size(); ++i)
        {
            const GSMobiSection &section = m_sections[i];
            GSMobiEntry node;
            node.offset = (int)section.htmlBeginPos;
            node.length = (int)section.htmlEndPos - node.offset;
            node.label = section.title;
            node.depth = 1;
            node.clazz = "section";
            node.parent = 0;
            node.child1 = articleIndex + 1;
            node.childN = articleIndex + (int)section.chapters.size();
            entries.push_back(node);
            articleIndex += (int)section.chapters.size();
        }
        // article
        for (size_t i = 0; i < m_sections.size(); ++i)
        {
            const GSMobiSection &section = m_sections[i];
            for (size_t j = 0; j < section.chapters.size(); ++j)
            {
                const GSMobiChapter &chapter = section.chapters[j];
                GSMobiEntry node;
                node.offset = (int)chapter.htmlBeginPos;
                node.length = (int)chapter.htmlEndPos - node.offset;
                node.label = chapter.title;
                node.depth = 2;
                node.clazz = "article";
                node.parent = (int)i + 1;
                node.author = "test author";
                node.description = "test desc";
                entries.push_back(node);
            }
        }
    }
    else
    {
    }
    return entries;
}

GSTagx GSMobiPacker::BuildSecondTagx()
{
    GSTagx tagx;
    tagx.AddTag(GS_TAGX_Secondary, 3, 0x01);
    tagx.AddTag(GS_TAGX_END, 0, 0x00);
    return tagx;
}

vector<GSMobiEntry> GSMobiPacker::BuildSecondEntries()
{
    vector<GSMobiEntry> entries;
    GSMobiEntry entry;
    entry.name = "author";
    entry.second1 = 0;
    entry.second2 = 0;
    entry.second3 = 71;
    entries.push_back(entry);

    entry.name = "description";
    entry.second3 = 70;
    entries.push_back(entry);

    entry.name = "mastheadImage";
    entry.second1 = 5;
    entry.second3 = 69;
    entries.push_back(entry);
    return entries;
}

GSBytes GSMobiPacker::BuildCNCX(vector<GSMobiEntry> & entries)
{
    // Entry name
    int indexLen = 0;
    for (size_t count = entries.size(); count; count >>= 4)
    {
        ++indexLen;
    }

    GSBytes bytes;
    map<string, int> strMap;
    map<string, int>::const_iterator mapIter = strMap.end();
    for (size_t i = 0; i < entries.size(); ++i)
    {
        GSMobiEntry &entry = entries[i];
        entry.name = GSStringF("%0*zX", indexLen, i);;
        if (!entry.label.empty())
        {
            mapIter = strMap.find(entry.label);
            if (mapIter == strMap.end())
            {
                entry.labelOffset = (int)bytes.size();
                GSPushString(bytes, entry.label);
                strMap[entry.label] = entry.labelOffset;
            }
            else
            {
                entry.labelOffset = mapIter->second;
            }
        }
        if (!entry.clazz.empty())
        {
            mapIter = strMap.find(entry.clazz);
            if (mapIter == strMap.end())
            {
                entry.classOffset = (int)bytes.size();
                GSPushString(bytes, entry.clazz);
                strMap[entry.clazz] = entry.classOffset;
            }
            else
            {
                entry.classOffset = mapIter->second;
            }
        }
        if (!entry.description.empty())
        {
            mapIter = strMap.find(entry.description);
            if (mapIter == strMap.end())
            {
                entry.descOffset = (int)bytes.size();
                GSPushString(bytes, entry.description);
                strMap[entry.description] = entry.descOffset;
            }
            else
            {
                entry.descOffset = mapIter->second;
            }
        }
        if (!entry.author.empty())
        {
            mapIter = strMap.find(entry.author);
            if (mapIter == strMap.end())
            {
                entry.authorOffset = (int)bytes.size();
                GSPushString(bytes, entry.author);
                strMap[entry.author] = entry.authorOffset;
            }
            else
            {
                entry.authorOffset = mapIter->second;
            }
        }
    }
    GSPushPadding(bytes);
    return bytes;
}

GSBytes GSMobiPacker::BuildINDXInfo(const vector<GSMobiEntry> & entries, const GSTagx & tagx)
{
    GSBytes lastEntryBytes;
    const GSMobiEntry &lastEntry = entries.back();
    lastEntryBytes.push_back((char)lastEntry.name.length());
    GSPushArray(lastEntryBytes, lastEntry.name.c_str(), lastEntry.name.length());
    GSPushU16BE(lastEntryBytes, (uint16_t)entries.size());
    GSPushPadding(lastEntryBytes);

    GSIndxHeader header;
    header.indexType = 2;
    header.idxtOffset = GS_INDX_HEADER_LEN + tagx.length + (uint32_t)lastEntryBytes.size();
    header.indexCount = 1;
    header.indexEncoding = 0xFDE9;
    header.entryCount = (uint32_t)entries.size();
    header.cncxCount = tagx.MaskForTag(GS_TAGX_Secondary) ? 0 : 1;
    header.tagxOffset = GS_INDX_HEADER_LEN;

    GSBytes bytes;
    header.WriteTo(bytes);
    tagx.WriteTo(bytes);
    bytes.insert(bytes.end(), lastEntryBytes.begin(), lastEntryBytes.end());
    GSPushU32BE(bytes, 'IDXT');
    GSPushU16BE(bytes, (uint16_t)(GS_INDX_HEADER_LEN + tagx.length));
    GSPushPadding(bytes);
    return bytes;
}

GSBytes GSMobiPacker::BuildINDXValue(const vector<GSMobiEntry> & entries, const GSTagx & tagx)
{
    GSBytes entriesBytes;
    vector<uint16_t> entryOffsets;
    for (size_t i = 0; i < entries.size(); ++i)
    {
        entryOffsets.push_back(GS_INDX_HEADER_LEN + (uint16_t)entriesBytes.size());
        const GSMobiEntry &entry = entries[i];
        entriesBytes.push_back((char)entry.name.length());
        GSPushArray(entriesBytes, entry.name.c_str(), entry.name.length());

        uint8_t bitFlag = 0;
        GSBytes entryBytes;
        if (entry.offset >= 0)
        {
            bitFlag |= tagx.MaskForTag(GS_TAGX_Offset);
            GSPushVWInt(entryBytes, entry.offset);
        }
        if (entry.length >= 0)
        {
            bitFlag |= tagx.MaskForTag(GS_TAGX_Length);
            GSPushVWInt(entryBytes, entry.length);
        }
        if (entry.labelOffset >= 0)
        {
            bitFlag |= tagx.MaskForTag(GS_TAGX_LabelOffset);
            GSPushVWInt(entryBytes, entry.labelOffset);
        }
        if (entry.depth >= 0)
        {
            bitFlag |= tagx.MaskForTag(GS_TAGX_Depth);
            GSPushVWInt(entryBytes, entry.depth);
        }
        if (entry.classOffset >= 0)
        {
            bitFlag |= tagx.MaskForTag(GS_TAGX_ClassOffset);
            GSPushVWInt(entryBytes, entry.classOffset);
        }
        if (entry.parent >= 0)
        {
            bitFlag |= tagx.MaskForTag(GS_TAGX_Parent);
            GSPushVWInt(entryBytes, entry.parent);
        }
        if (entry.child1 >= 0)
        {
            bitFlag |= tagx.MaskForTag(GS_TAGX_Child1);
            GSPushVWInt(entryBytes, entry.child1);
        }
        if (entry.childN >= 0)
        {
            bitFlag |= tagx.MaskForTag(GS_TAGX_ChildN);
            GSPushVWInt(entryBytes, entry.childN);
        }
        if (entry.second1 >= 0 && entry.second2 >= 0 && entry.second3 >= 0)
        {
            bitFlag |= tagx.MaskForTag(GS_TAGX_Secondary);
            GSPushVWInt(entryBytes, entry.second1);
            GSPushVWInt(entryBytes, entry.second2);
            GSPushVWInt(entryBytes, entry.second3);
        }
        entriesBytes.push_back((char)bitFlag);
        if (tagx.controlByteCount > 1)
        {
            bitFlag = 0;
            if (entry.imageIndex >= 0)
            {
                bitFlag |= tagx.MaskForTag(GS_TAGX_ImageIndex);
                GSPushVWInt(entryBytes, entry.imageIndex);
            }
            if (entry.descOffset >= 0)
            {
                bitFlag |= tagx.MaskForTag(GS_TAGX_DescOffset);
                GSPushVWInt(entryBytes, entry.descOffset);
            }
            if (entry.authorOffset >= 0)
            {
                bitFlag |= tagx.MaskForTag(GS_TAGX_AuthorOffset);
                GSPushVWInt(entryBytes, entry.authorOffset);
            }
            entriesBytes.push_back((char)bitFlag);
        }
        entriesBytes.insert(entriesBytes.end(), entryBytes.begin(), entryBytes.end());
    }
    GSPushPadding(entriesBytes);

    GSIndxHeader header;
    header.unknown1 = 1;
    header.idxtOffset = GS_INDX_HEADER_LEN + (uint32_t)entriesBytes.size();
    header.indexCount = (uint32_t)entries.size();

    GSBytes bytes;
    header.WriteTo(bytes);
    bytes.insert(bytes.end(), entriesBytes.begin(), entriesBytes.end());
    GSPushU32BE(bytes, 'IDXT');
    for (size_t i = 0; i < entryOffsets.size(); ++i)
    {
        GSPushU16BE(bytes, entryOffsets[i]);
    }
    GSPushPadding(bytes);
    return bytes;
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
    exthHeader.recordCount = (uint32_t)m_exthRecords.size();
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
