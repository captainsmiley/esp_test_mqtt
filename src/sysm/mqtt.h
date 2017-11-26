#ifndef MQTT_TTG
#define MQTT_TTG

#include <PubSubClient.h>
#include <ESP8266WiFi.h>


class Mqtt_manager
{
public:
  Mqtt_manager();
  void setup();
  void update();

private:
  static void callback(char* topic, byte* payload, unsigned int length);
  static PubSubClient client;
  void reconnect();



};


#endif
