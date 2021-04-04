
#ifndef _PUB_SUB
#define _PUB_SUB
struct data {    
    enum ptypes type;    
    void *value;    
    
    void (*getCB)(void *) ;    
    void (*setCB)(void *) ;    
    void (*subCB)(void*) ;    
    void (*unsubCB)(void *) ;    
    
};

void setBoolean(KVSstore *s, char *name, bool state);

#endif

