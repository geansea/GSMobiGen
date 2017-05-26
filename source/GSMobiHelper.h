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
    uint16_t unused;         // [02h~04h]: 
    uint32_t textlength;     // [04h~08h]: 
    uint16_t recordCount;    // [08h~0Ah]: 
    uint16_t recordSize;     // [0Ah~0Ch]: 0x1000
    uint16_t encryptionType; // [0Ch~0Eh]: 0 - no enc, 1 - Old Mobipocket Encryption, 2 - Mobipocket Encryption
    uint16_t reserve;        // [0Eh~10h]: 

    // Default values for MOBI
    GSPalmDocHeader();
    void ReadFrom(const char *p);
    void WriteTo(GSBytes & bytes) const;
};

#define GS_PALM_DOC_HEADER_LEN 0x10

enum
{
    GS_MOBI_TYPE_MOBIPOCKET_BOOK = 0x0002,
    GS_MOBI_TYPE_NEWS_MAGAZINE   = 0x0103,
};

struct GSMobiHeader
{
    uint32_t identifier;          // [00h~04h]: 'MOBI'
    uint32_t headerLength;        // [04h~08h]: 
    uint32_t mobiType;            // [08h~0Ch]: 
    uint32_t textEncoding;        // [0Ch~10h]: 0x04E4(1252) - CP1252(WinLatin1), 0xFDE9 - UTF-8
    uint32_t uniqueID;            // [10h~14h]: 
    uint32_t fileVersion;         // [14h~18h]: 
    uint32_t ortographicIndex;    // [18h~1Ch]: Section number of orthographic meta index
    uint32_t inflectionIndex;     // [1Ch~20h]: Section number of inflection meta index
    uint32_t indexNames;          // [20h~24h]: 
    uint32_t indexKeys;           // [24h~28h]: 
    uint32_t extraIndex0;         // [28h~2Ch]: 
    uint32_t extraIndex1;         // [2Ch~30h]: 
    uint32_t extraIndex2;         // [30h~34h]: 
    uint32_t extraIndex3;         // [34h~38h]: 
    uint32_t extraIndex4;         // [38h~3Ch]: 
    uint32_t extraIndex5;         // [3Ch~40h]: 
    uint32_t firstNonBookIndex;   // [40h~44h]: 
    uint32_t fullNameOffset;      // [44h~48h]: 
    uint32_t fullNameLength;      // [48h~4Ch]: 
    uint32_t locale;              // [4Ch~50h]: 
    uint32_t inputLanguage;       // [50h~54h]: 
    uint32_t outputLanguage;      // [54h~58h]: 
    uint32_t minVersion;          // [58h~5Ch]: 
    uint32_t firstImageIndex;     // [5Ch~60h]: 
    uint32_t huffmanRecordOffset; // [60h~64h]: 
    uint32_t huffmanRecordCount;  // [64h~68h]: 
    uint32_t huffmanTableOffset;  // [68h~6Ch]: 
    uint32_t huffmanTableLength;  // [6Ch~70h]: 
    uint32_t exthFlags;           // [70h~74h]: 
    uint32_t unknown0;            // [74h~78h]: 
    uint32_t unknown1;            // [78h~7Ch]: 
    uint32_t unknown2;            // [7Ch~80h]: 
    uint32_t unknown3;            // [80h~84h]: 
    uint32_t unknown4;            // [84h~88h]: 
    uint32_t unknown5;            // [88h~8Ch]: 
    uint32_t unknown6;            // [8Ch~90h]: 
    uint32_t unknown7;            // [90h~94h]: 
    uint32_t drmOffset;           // [94h~98h]: 
    uint32_t drmCount;            // [98h~9Ch]: 
    uint32_t drmSize;             // [9Ch~A0h]: 
    uint32_t drmFlags;            // [A0h~A4h]: 
    uint32_t unknown8;            // [A4h~A8h]: 
    uint32_t unknown9;            // [A8h~ACh]: 
    uint32_t unknown10;           // [ACh~B0h]: 
    uint16_t firstContentIndex;   // [B0h~B2h]: 1
    uint16_t lastContentIndex;    // [B2h~B4h]: 
    uint32_t unknown11;           // [B4h~B8h]: 1
    uint32_t fcisIndex;           // [B8h~BCh]: 
    uint32_t fcisCount;           // [BCh~C0h]: 1
    uint32_t flisIndex;           // [C0h~C4h]: 
    uint32_t flisCount;           // [C4h~C8h]: 1
    uint32_t unknown12;           // [C8h~CCh]: 
    uint32_t unknown13;           // [CCh~D0h]: 
    uint32_t srcsIndex;           // [D0h~D4h]: 0xFFFFFFFF
    uint32_t srcsCount;           // [D4h~D8h]: 0
    uint32_t unknown14;           // [D8h~DCh]: 0xFFFFFFFF
    uint32_t unknown15;           // [DCh~E0h]: 0xFFFFFFFF
    uint32_t extraDataFlags;      // [E0h~E4h]: 
    uint32_t indxRecordOffset;    // [E4h~E8h]: 

    // Default values for MOBI
    GSMobiHeader();
    void ReadFrom(const char *p);
    void WriteTo(GSBytes & bytes) const;
};

#define GS_MOBI_HEADER_LEN 0xE8

struct GSExthRecord
{
    uint32_t type;        // [00h~04h]: 
    uint32_t recordLen;   // [04h~08h]: 
    char *   pRecordData; // size: recordLen - 8
};

struct GSExthHeader
{
    uint32_t    identifier;   // [00h~04h]: 'EXTH'
    uint32_t    headerLength; // [04h~08h]: 
    uint32_t    recordCount;  // [08h~0Ch]: 
    GSExthRecord * pRecords;
    // padding
};

#endif /* GSMobiHelper_h */
