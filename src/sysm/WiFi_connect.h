#ifndef WIFI_CON_TTGG
#define WIFI_CON_TTGG

#include <ESP8266WiFi.h>

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


#define WIFI_CON_UPDATE_RATE_MS 200



class WiFi_connecter
{
public:
  const static uint16_t update_rate = WIFI_CON_UPDATE_RATE_MS;
  WiFi_connecter();
  virtual ~WiFi_connecter();

  void update();
  void setup();

  void debugg();

  static WiFiClient client;
  private:
    void print_wl_status();
    void find_clients();
    void check_OTA();
     const char* ap_ssid;
     const char* ap_password;

};







#endif /* end of include guard:  */
