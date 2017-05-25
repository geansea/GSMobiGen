#ifndef IGSMobiPacker_h
#define IGSMobiPacker_h

class IGSMobiPacker
{
public:
    static IGSMobiPacker * Create();

public:
    virtual ~IGSMobiPacker() {}

public:
    virtual void AddRecord(const GSBytes & record) = 0;
    virtual bool WriteTo(const char * pFileName) = 0;
};

#endif /* IGSMobiPacker_h */
