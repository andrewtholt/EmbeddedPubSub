
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "Small.h"
#include "cmdParser.h"
#include "linuxParser.h"

extern struct linuxParser *newParser();

int main() {
    int rc=0;
    bool failFlag=true;
    struct Small *db;
    struct cmdMessage msg;
    struct linuxParser *p;
    
    memset(&msg, 0, sizeof(msg));

    db=newSmall();
    p=newParser(db);
    
    msg.message.fields = 3;
    strcpy(msg.message.cmd,"SET");
    strcpy(msg.message.key,"DEBUG");
    strcpy(msg.message.value,"TRUE");
    
    failFlag=cmdParse(p,&msg);
    
    
    return rc;
}
