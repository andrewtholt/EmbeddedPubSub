
#include "db.h"

class espDb : public db {
    private:
        dbError wifiConnect();
    public:
        virtual void act(KVSkey *k,KVSvalue *v);
};

