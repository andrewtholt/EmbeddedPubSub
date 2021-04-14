#include "Small.h"

int main() {
    Small *myDb = new Small();

    bool failFlag = myDb->dbInstall("TEST","ING");  
    failFlag = myDb->dbInstall("FRED","TEST_F");  

    printf("%d\n", failFlag);

    printf("TEST = %s\n", myDb->dbLookup("TEST"));

    nlist *rec;

    rec = myDb->dbLookupRec("TEST");

    myDb->printDb();
}

