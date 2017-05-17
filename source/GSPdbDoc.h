#ifndef GSPdbDoc_h
#define GSPdbDoc_h

#include "GSPdbHelper.h"

class GSPdbDoc
{
public:
    GSPdbDoc();
    ~GSPdbDoc();

public: // Read
    bool Open(const string &fileName);
    int RecordCount();
    const GSBytes * GetRecord(int index);

public: // Write
    void SetHeader(const GSPdbHeader &header);
    void AddRecord(const GSBytes &record);
    bool WriteTo(const string &fileName);

private:
    GSPdbHeader     m_header;
    GSPdbRecordList m_recordList;
    vector<GSBytes> m_records;
};

#endif /* GSPdbDoc_h */
