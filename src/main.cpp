#include <Arduino.h>
#include <TickerScheduler.h>

#include <ESP8266WiFi.h>

#include "conf.h"

#include "sysm/WiFi_connect.h"
#if MQTT_ENABLED
#include "sysm/mqtt.h"
#endif
#include "sig/signals.h"
#include "sysm/OTA.h"
#include "app/test_mqtt.h"
#if COMMANDS_ENABLED
#include "com/tgesp.h"
#endif
TickerScheduler ts(5);

Signals sig;

WiFi_connecter wc(sig);
void wifi_con_update()
{
  wc.update();
}
OTA ota;
void ota_update()
{
  ota.update();
}
#if MQTT_ENABLED
Mqtt_manager mqttm;
void mqtt_update()
{
  mqttm.update();
}
#endif

#if COMMANDS_ENABLED
tgesp com;

void com_update()
{
  com.update();
}
#endif


unsigned long int t_g;


int loop_counter = 0;


void print_info()
{
  Serial.print("T:");Serial.print(t_g++);Serial.print(" V:");Serial.print(TG_VERSION);
  Serial.print("| ");wc.debugg();
  Serial.print("| Free flash:");Serial.print(ESP.getFreeSketchSpace());
  Serial.print(" Free heap:");Serial.print(ESP.getFreeHeap());

  Serial.println("");

}

void app()
{

  //mqttm.client.publish("tgtest","hej tgtest");
  //mqttm.add_sub_topic(tgtest_topic,&handle_tgtest );
}


void setup()
{

  Serial.begin(115200);
  Serial.println("Start of main");

  // Setup components
  wc.setup();
  ota.setup();
  #if MQTT_ENABLED
  mqttm.setup();
  ts.add(2,mqttm.update_rate,mqtt_update);
  #endif
  #if COMMANDS_ENABLED
  com.setup();
  ts.add(3,com.update_rate,com_update);
  #endif


  ts.add(1, wc.update_rate,wifi_con_update);
  ts.add(0,ota.update_rate,ota_update);
  ts.add(4,1000,print_info);



}


void loop()
{
  ts.update();

}
