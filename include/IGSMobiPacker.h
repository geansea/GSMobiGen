#ifndef IGSMobiPacker_h
#define IGSMobiPacker_h

class IGSMobiPacker
{
public:
    static IGSMobiPacker * Create();

public:
    virtual ~IGSMobiPacker() {}

public:
    virtual bool WriteTo(const char * pFileName) = 0;
};

#endif /* IGSMobiPacker_h */