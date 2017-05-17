#ifndef GSPdbDoc_h
#define GSPdbDoc_h

#include "GSMobiHelper.h"

class GSPdbDoc
{
public:
    GSPdbDoc();
    ~GSPdbDoc();

public: // Read
    bool Open(string fileName);
    int RecordCount();
    const GS_BYTES * GetRecord(int index);

public: // Write

private:
    int m_recordCount;
    vector<GS_BYTES> m_records;
};

#endif /* GSPdbDoc_h */
