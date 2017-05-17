#ifndef GSPdbDoc_h
#define GSPdbDoc_h

#include <string>
#include <vector>

class GSPdbDoc
{
public:
    GSPdbDoc();
    ~GSPdbDoc();

public: // Read
    bool Open(std::string fileName);
    int RecordCount();
    const std::vector<unsigned char> * GetRecord(int index);

public: // Write

private:
};

#endif /* GSPdbDoc_h */
