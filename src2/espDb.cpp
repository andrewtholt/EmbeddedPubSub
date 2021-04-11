#include <iostream>

/***********************************************************************
 * AUTHOR: andrewh <andrewh>
 *   FILE: .//espDb.cpp
 *   DATE: Fri Oct 23 10:10:59 2020
 *  DESCR: 
 ***********************************************************************/
#include "espDb.h"

/***********************************************************************
 *  Method: espDb::act
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void espDb::act(KVSkey *k,KVSvalue *v) {

    dbError err;
    std::cout << "Overiding act()\n";
    if(!strcmp((char *)v,"ON")) {    
        //
        // might have an action, so check key.    
        //    
        if(!strcmp((char *)k,"WIFI_CONNECT")) {    
            std::cout << "WIFI_CONNECT\n" ;    

            err = wifiConnect();    
        }    
    }
}


/***********************************************************************
 *  Method: espDb::wifiConnect
 *  Params: 
 * Returns: dbError
 * Effects: 
 ***********************************************************************/
dbError espDb::wifiConnect() {
    dbError err = GENERAL_ERROR;

    kvs_put(store, "WIFI_CONNECT",(KVSvalue *)"OFF");

    const KVSvalue *ssid = get("WIFI_SSID");
    const KVSvalue *passwd = nullptr;

    if(ssid == nullptr) {
        err=NO_SSID;
    } else {
        err=OK;
        passwd = get("WIFI_PASSWD");
        if(passwd == nullptr) {
            err = NO_PASSWD;
        } else {
            err=OK;

            std::cout << "Connecting to WiFi\n" ;
        }
    }

    return err;
}


