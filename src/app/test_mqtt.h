#ifndef APP_TEST_MQTT_TTG
#define APP_TEST_MQTT_TTG
#include <ESP8266WiFi.h>

#include "sysm/mqtt.h"
#define TEST_MQTT_UPDATE_RATE_MS 100

class Test_mqtt
{
public:
  Test_mqtt(Mqtt_manager &);
  virtual ~Test_mqtt();

  const static uint8_t update_rate = TEST_MQTT_UPDATE_RATE_MS;
  void update();
  void setup();
private:
  Mqtt_manager mqttm;
  String msg;


};


#endif
