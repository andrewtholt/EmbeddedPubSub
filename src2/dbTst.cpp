//
// #include "db.h"
#include "espDb.h"
#include <iostream>

using namespace std;

int main() {

//    db *data = new db;
    espDb *data = new espDb;

    data->set("WIFI_SSID",(KVSvalue *)"HoltAtHome4",false);
    data->set("WIFI_PASSWD",(KVSvalue *)"fred",false);

    data->set("HOST_IP",(KVSvalue *)"0.0.0.0",false);
    data->set("HOST_PORT",(KVSvalue *)"0.0.0.0",false);

    data->set("MQTT_HOST",(KVSvalue *)"0.0.0.0",false);
    data->set("MQTT_PORT",(KVSvalue *)"1883",false);

    data->set("AUTO_CONNECT",(KVSvalue *)"OFF",false);
    data->set("SLEEP",(KVSvalue *)"0",false);

    data->set("AUTO_SAVE",(KVSvalue *)"NO",false);
    data->set("CONFIGURED",(KVSvalue *)"NO",false);

    data->set("WIFI_CONNECT",(KVSvalue *)"OFF",false);
    data->set("WIFI_CONNECTED",(KVSvalue *)"OFF",false);

    data->set("MQTT_CONNECT",(KVSvalue *)"OFF",false);
    data->set("MQTT_CONNECTED",(KVSvalue *)"OFF",false);

    data->set("CFG_CHANGED",(KVSvalue *)"OFF",false);
    data->set("SAVE",(KVSvalue *)"OFF",false);
    // 
    //

    data->set("WIFI_CONNECT",(KVSvalue *)"ON");

    const KVSvalue *p = data->get("WIFI_CONNECT");

    cout << "Value   : " << (char *)p << endl;

    cout << "Entries : " << data->count() << endl;

}

