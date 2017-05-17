#ifndef GSPdbDoc_h
#define GSPdbDoc_h

#include "GSPdbHelper.h"

class GSPdbDoc
{
public:
    GSPdbDoc();
    ~GSPdbDoc();

public: // Read
    bool Open(string fileName);
    int RecordCount();
    const GSBytes * GetRecord(int index);

public: // Write
    void SetHeader(const PdbHeader &header);

private:
    PdbHeader  m_header;
    RecordList m_recordList;
};

#endif /* GSPdbDoc_h */
