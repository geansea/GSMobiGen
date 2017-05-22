#ifndef GSPdbDoc_h
#define GSPdbDoc_h

#include "GSPdbHelper.h"

class GSPdbDoc
{
public:
    GSPdbDoc();
    ~GSPdbDoc();

public: // Read
    bool Open(const string & fileName);
    int RecordCount();
    const GSBytes * GetRecordData(int index);

public: // Write
    void SetHeader(const GSPdbHeader & header);
    void AddRecordData(const GSBytes & recordData);
    bool WriteTo(const string & fileName);

private:
    bool ReadFile(const string & fileName, GSBytes & bytes);
    bool WriteFile(const string & fileName, const GSBytes & bytes);

private:
    GSPdbHeader     m_header;
    GSPdbRecordList m_recordList;
    vector<GSBytes> m_recordDatas;
};

#endif /* GSPdbDoc_h */
