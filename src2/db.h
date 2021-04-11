#include "kvs.h"
#include <stdint.h>
#include <iostream>

enum dbError {
    OK=0,
    NO_SSID,
    NO_PASSWD,
    GENERAL_ERROR=0xfe
};
class db {
    protected:
        KVSstore *store = nullptr;

    public:
        db();
        const KVSvalue *get(const char *k);
        void set(const char *k, KVSvalue *v);
        void set(const char *k, KVSvalue *v, bool action);
        int count();

        virtual void act(KVSkey *k,KVSvalue *v) { std::cout << "Original act\n"; };
};

