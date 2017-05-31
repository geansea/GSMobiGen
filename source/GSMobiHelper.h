#ifndef GSMobiHelper_h
#define GSMobiHelper_h

#include "GSPdbHelper.h"

// Based on http://wiki.mobileread.com/wiki/MOBI

enum
{
    GS_MOBI_COMP_NONE = 1,
    GS_MOBI_COMP_LZ77 = 2,
    GS_MOBI_COMP_HUFF = 0x4448,
};

struct GSPalmDocHeader
{
    uint16_t compression;    // [00h~02h]: 1 - no comp, 2 - PalmDOC(LZ77), 0x4448 - HUFF/CDIC
    uint16_t unused;         // [02h~04h]: 0
    uint32_t textlength;     // [04h~08h]: 
    uint16_t recordCount;    // [08h~0Ah]: 
    uint16_t recordSize;     // [0Ah~0Ch]: 0x1000
    uint16_t encryptionType; // [0Ch~0Eh]: 0 - no enc, 1 - Old Mobipocket Encryption, 2 - Mobipocket Encryption
    uint16_t reserve;        // [0Eh~10h]: 0

    GSPalmDocHeader();
    void ReadFrom(const char *& p);
    void WriteTo(GSBytes & bytes) const;
};

#define GS_PALM_DOC_HEADER_LEN 0x10

struct GSMobiHeader
{
    uint32_t identifier;          // [00h~04h]: 'MOBI'
    uint32_t headerLength;        // [04h~08h]: 0xE8
    uint32_t mobiType;            // [08h~0Ch]: GS_MOBI_TYPE
    uint32_t textEncoding;        // [0Ch~10h]: 0x04E4(1252) - CP1252(WinLatin1), 0xFDE9 - UTF-8
    uint32_t uniqueID;            // [10h~14h]: rand()
    uint32_t fileVersion;         // [14h~18h]: 6
    uint32_t ortographicIndex;    // [18h~1Ch]: 0xFFFFFFFF
    uint32_t inflectionIndex;     // [1Ch~20h]: 0xFFFFFFFF
    uint32_t indexNames;          // [20h~24h]: 
    uint32_t indexKeys;           // [24h~28h]: 0xFFFFFFFF
    uint32_t extraIndex0;         // [28h~2Ch]: 0xFFFFFFFF
    uint32_t extraIndex1;         // [2Ch~30h]: 0xFFFFFFFF
    uint32_t extraIndex2;         // [30h~34h]: 0xFFFFFFFF
    uint32_t extraIndex3;         // [34h~38h]: 0xFFFFFFFF
    uint32_t extraIndex4;         // [38h~3Ch]: 0xFFFFFFFF
    uint32_t extraIndex5;         // [3Ch~40h]: 0xFFFFFFFF
    uint32_t firstNonBookIndex;   // [40h~44h]: GSPalmDocHeader.recordCount + (1 or 2)
    uint32_t fullNameOffset;      // [44h~48h]: GS_PALM_DOC_HEADER_LEN + GS_MOBI_HEADER_LEN + GSExthHeader.headerLength
    uint32_t fullNameLength;      // [48h~4Ch]: 
    uint32_t locale;              // [4Ch~50h]: GS_MOBI_LANGUAGE
    uint32_t inputLanguage;       // [50h~54h]: 0
    uint32_t outputLanguage;      // [54h~58h]: 0
    uint32_t minVersion;          // [58h~5Ch]: 6
    uint32_t firstImageIndex;     // [5Ch~60h]: 
    uint32_t huffmanRecordOffset; // [60h~64h]: 0
    uint32_t huffmanRecordCount;  // [64h~68h]: 0
    uint32_t huffmanTableOffset;  // [68h~6Ch]: 0
    uint32_t huffmanTableLength;  // [6Ch~70h]: 0
    uint32_t exthFlags;           // [70h~74h]: 0x50
    uint32_t unknown0;            // [74h~78h]: 0
    uint32_t unknown1;            // [78h~7Ch]: 0
    uint32_t unknown2;            // [7Ch~80h]: 0
    uint32_t unknown3;            // [80h~84h]: 0
    uint32_t unknown4;            // [84h~88h]: 0
    uint32_t unknown5;            // [88h~8Ch]: 0
    uint32_t unknown6;            // [8Ch~90h]: 0
    uint32_t unknown7;            // [90h~94h]: 0
    uint32_t drmOffset;           // [94h~98h]: 0xFFFFFFFF
    uint32_t drmCount;            // [98h~9Ch]: 0xFFFFFFFF
    uint32_t drmSize;             // [9Ch~A0h]: 0
    uint32_t drmFlags;            // [A0h~A4h]: 0
    uint32_t unknown8;            // [A4h~A8h]: 0
    uint32_t unknown9;            // [A8h~ACh]: 0
    uint32_t unknown10;           // [ACh~B0h]: 0
    uint16_t firstContentIndex;   // [B0h~B2h]: 1
    uint16_t lastContentIndex;    // [B2h~B4h]: Last image record
    uint32_t unknown11;           // [B4h~B8h]: 1
    uint32_t fcisIndex;           // [B8h~BCh]: 
    uint32_t fcisCount;           // [BCh~C0h]: 1
    uint32_t flisIndex;           // [C0h~C4h]: 
    uint32_t flisCount;           // [C4h~C8h]: 1
    uint32_t unknown12;           // [C8h~CCh]: 0
    uint32_t unknown13;           // [CCh~D0h]: 0
    uint32_t srcsIndex;           // [D0h~D4h]: 0xFFFFFFFF
    uint32_t srcsCount;           // [D4h~D8h]: 0
    uint32_t unknown14;           // [D8h~DCh]: 0xFFFFFFFF
    uint32_t unknown15;           // [DCh~E0h]: 0xFFFFFFFF
    uint32_t extraDataFlags;      // [E0h~E4h]: 0x01 or 0x03 or 0x0B (?)
    uint32_t indxRecordOffset;    // [E4h~E8h]: 

    GSMobiHeader();
    void ReadFrom(const char *& p);
    void WriteTo(GSBytes & bytes) const;
};

#define GS_MOBI_HEADER_LEN 0xE8

struct GSExthHeader
{
    uint32_t identifier;   // [00h~04h]: 'EXTH'
    uint32_t headerLength; // [04h~08h]: 
    uint32_t recordCount;  // [08h~0Ch]: 
    // padding

    GSExthHeader();
    void ReadFrom(const char *& p);
    void WriteTo(GSBytes & bytes) const;
};

#define GS_EXTH_HEADER_LEN 0x0C

enum GS_MOBI_EXTH_INNER_TYPE
{
    GS_MOBI_EXTH_START_OFFSET      = 0x0074,
    GS_MOBI_EXTH_COVER_INDEX       = 0x00C9, // Based on firstImageIndex
    GS_MOBI_EXTH_THUMB_INDEX       = 0x00CA, // Based on firstImageIndex
    GS_MOBI_EXTH_HAS_FAKE_COVER    = 0x00CB, // 0
    GS_MOBI_EXTH_CREATOR_SOFTWARE  = 0x00CC, // 200 - kindlegen Windows, 201 - kindlegen Linux, 202 - kindlegen Mac
    GS_MOBI_EXTH_CREATOR_MAJOR_VER = 0x00CD, // 1
    GS_MOBI_EXTH_CREATOR_MINOR_VER = 0x00CE, // 1
    GS_MOBI_EXTH_CREATOR_BUILD_NUM = 0x00CF, // 99
};

struct GSExthRecord
{
    uint32_t type;   // [00h~04h]: 
    uint32_t length; // [04h~08h]: 
    GSBytes  data;   // size: length - 8

    GSExthRecord();
    void ReadFrom(const char *& p);
    void WriteTo(GSBytes & bytes) const;
};

struct GSMobiChapter
{
    string title;
    string content;

    GSMobiChapter();
    void SetPureTextContent(const string & text);
};

struct GSMobiSection
{
    string                 title;
    vector<GSMobiChapter> chapters;

    GSMobiSection();
};

#endif /* GSMobiHelper_h */
