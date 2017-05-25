#ifndef GSMobiHelper_h
#define GSMobiHelper_h

#include "GSPdbHelper.h"

// Based on http://wiki.mobileread.com/wiki/MOBI

struct GSPalmDocHeader
{
    uint16_t compression;    // [00h~02h]: 1 - no comp, 2 - PalmDOC(LZ77), 0x4448 - HUFF/CDIC
    uint16_t unused;         // [02h~04h]
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

#define GS_MOBI_TYPE_MOBIPOCKET_BOOK 0x0002
// #define MOBI_TYPE_PALMDOC_BOOK    0x0003
// #define MOBI_TYPE_AUDIO           0x0004
// #define MOBI_TYPE_NEWS            0x0101
// #define MOBI_TYPE_NEWS_FEED       0x0102
#define GS_MOBI_TYPE_NEWS_MAGAZINE   0x0103
// #define MOBI_TYPE_PICS            0x0201
// #define MOBI_TYPE_WORD            0x0202
// #define MOBI_TYPE_XLS_BOOK        0x0203
// #define MOBI_TYPE_PPT             0x0204
// #define MOBI_TYPE_TEXT            0x0205
// #define MOBI_TYPE_HTML            0x0206

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
    uint32_t extraIndex[6];       // [28h~40h]: Section number of extra meta index
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
    uint32_t huffmanTableOffset;  // [68h~6Ch]
    uint32_t huffmanTableLength;  // [6Ch~70h]
    uint32_t exthFlags;           // [70h~74h]: 
    uint32_t unknownBytes1[8];    // [74h~94h]: 
    uint32_t drmOffset;           // [94h~98h]
    uint32_t drmCount;            // [98h~9Ch]
    uint32_t drmSize;             // [9Ch~A0h]
    uint32_t drmFlags;            // [A0h~A4h]
    uint32_t unknownBytes2[15];   // [A4h~E0h]: 
    uint16_t unknownBytes3;       // [E0h~E2h]: 
    uint16_t extraDataFlags;      // [E2h~E4h]: 
    uint32_t indxRecordOffset;    // [E4h~E8h]: 

    // Default values for MOBI
    GSMobiHeader();
    void ReadFrom(const char *p);
    void WriteTo(GSBytes & bytes) const;
};

#define GS_MOBI_HEADER_LEN 0xE8

struct GSExthRecord
{
    uint32_t recordType;  // 
    uint32_t recordLen;   // 
    char *   pRecordData; // size: recordLen - 8
};

struct GSExthHeader
{
    char        identifier[4]; // 'EXTH'
    uint32_t    headerLength;  // 
    uint32_t    recordCount;   // 
    GSExthRecord * pRecords;
    // padding
};

#endif /* GSMobiHelper_h */
