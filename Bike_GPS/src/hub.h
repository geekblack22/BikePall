#include <WiFi.h>
#include "AzureIotHub.h"
#include "Esp32MQTTClient.h"

#define INTERVAL 10000
#define DEVICE_ID "Esp32Device"
#define MESSAGE_MAX_LEN 256

class Hub{
// Please input the SSID and password of WiFi
private:
    const char* ssid     = "NETGEAR24";
    const char* password = "grandtrail110";
    bool hasIoTHub = false;
/*String containing Hostname, Device Id & Device Key in the format:                         */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessKey=<device_key>"                */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessSignature=<device_sas_token>"    */
public:
     const char* connectionString = "HostName=BikePal.azure-devices.net;DeviceId=ESP-PAL;SharedAccessKey=7IrpFWkMjrOcBJzSjaiccO7cNmYxg/EChTA04xD/aqA=";

    void setup();
    void loop(char buff[100]);

   

};
