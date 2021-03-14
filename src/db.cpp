/***********************************************************************
 * AUTHOR: andrewh <andrewh>
 *   FILE: .//db.cpp
 *   DATE: Thu Oct 22 10:30:24 2020
 *  DESCR: 
 ***********************************************************************/
#include "db.h"
#include <iostream>

/***********************************************************************
 *  Method: db::db
 *  Params: 
 * Effects: 
 ***********************************************************************/
db::db() {
    store = kvs_create(strcmp);
}


/***********************************************************************
 *  Method: db::get
 *  Params: const char *k
 * Returns: const char *
 * Effects: 
 ***********************************************************************/
const KVSvalue *db::get(const char *k) {
    KVSvalue *ret = nullptr;

    ret = kvs_get(store, k);

    return ret;
}


/***********************************************************************
 *  Method: db::set
 *  Params: const char *k, KVSvalue *v
 * Returns: void
 * Effects: 
 ***********************************************************************/
void db::set(const char *k, KVSvalue *v) {
    dbError err = GENERAL_ERROR;

    kvs_put(store, (KVSkey *)k, (KVSvalue *)v);

    act(k,v);
}

/***********************************************************************
 *  Method: db::set
 *  Params: const char *k, KVSvalue *v, bool action
 * Returns: void
 * Effects: 
 ***********************************************************************/
void db::set(const char *k, KVSvalue *v, bool action) {
    kvs_put(store, (KVSkey *)k, (KVSvalue *)v);

    if(action) {
        act(k,v);
    }
}


/***********************************************************************
 *  Method: db::count
 *  Params: 
 * Returns: int
 * Effects: 
 ***********************************************************************/
int db::count() {
    return ( kvs_length(store));
}


