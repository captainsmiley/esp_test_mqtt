#ifndef WIFI_CON_TTGG
#define WIFI_CON_TTGG

#include <ESP8266WiFi.h>

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "sysm/signals.h"


#define WIFI_CON_UPDATE_RATE_MS 5000

#define AVOID_WIFI_COUNT 5



class WiFi_connecter
{
public:
  bool ap_ok;
  bool sta_ok;
  const static uint16_t update_rate = WIFI_CON_UPDATE_RATE_MS;
  WiFi_connecter(Signals & s);
  virtual ~WiFi_connecter();

  void update();
  void setup();

  void debugg();


  bool sta_con_main();
  static WiFiClient client;
  private:
    int select_wifi_to_connect(int n);
    void find_sta_and_connect();
    void connect_to_main_sta();
    void check_wifi_connections();
    Signals  & sig;
    void print_wl_status();
    void find_clients();
    void check_OTA();
    bool sta_in_avoid_list(String &s) const;
     const char* ap_ssid;
     const char* ap_password;

     String avoid_wifi_list[AVOID_WIFI_COUNT];
     String prefered_wifi;
     String try_out_wifi_string;

};







#endif /* end of include guard:  */
