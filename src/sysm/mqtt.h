#ifndef MQTT_TTG
#define MQTT_TTG

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define MQTT_UPDATE_RATE_MS 100
#define NR_MAX_SUBS 10

class Mqtt_manager
{
public:
  Mqtt_manager();
  void setup();
  void update();
  const static uint8_t update_rate = MQTT_UPDATE_RATE_MS;
  typedef void (*f_ptr_callback) (const char*, byte*, unsigned int);
  void add_sub_topic(const char* topic, f_ptr_callback c);
  static PubSubClient client;



private:
  static void callback(char* topic, byte* payload, unsigned int length);
  void reconnect();
  static uint8_t nr_subs;
  static f_ptr_callback callbacks[NR_MAX_SUBS];
  static const char* topics[NR_MAX_SUBS];

};


#endif
