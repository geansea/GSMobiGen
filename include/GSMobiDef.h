#ifndef GSMobiDef_h
#define GSMobiDef_h

#include <string>
#include <vector>

using namespace std;

typedef vector<char> GSBytes;

inline uint16_t GSGetU16BE(const char *& p) {
    uint16_t u = ((uint16_t)p[0] << 8) 
               + ((uint16_t)p[1]     );
    p += 2;
    return u;
}

inline uint32_t GSGetU32BE(const char *& p) {
    uint32_t u = ((uint32_t)p[0] << 24) 
               + ((uint32_t)p[1] << 16) 
               + ((uint32_t)p[2] <<  8) 
               + ((uint32_t)p[3]      );
    p += 4;
    return u;
}

inline void PutU16BE(uint16_t u, char *& p)
{
    *(p++) = (char)(u >> 8);
    *(p++) = (char)(u     );
}

inline void PutU32BE(uint32_t u, char *& p)
{
    *(p++) = (char)(u >> 24);
    *(p++) = (char)(u >> 16);
    *(p++) = (char)(u >>  8);
    *(p++) = (char)(u      );
}

#endif /* GSMobiDef_h */
