#include <stdio.h>    
#include <stdint.h>    
#include <stdbool.h>    
#include <pthread.h>     /* pthread functions and data structures */    
    
#include "kvs.h"    
#include "proto.h"    
#include "pubSub.h"    
#include "msg.h"    
#include "dump.h"    
#include <unistd.h>  
#include "db.h"

int main() {

    int rc = 1;

    void *db = dbCreate();


    return rc;
}
