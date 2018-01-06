#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <TickerScheduler.h>

#include "sysm/WiFi_connect.h"
#include "sysm/mqtt.h"

#include "conf.h"



#include "app/test_mqtt.h"

#define TG_VERSION 8

TickerScheduler ts(5);


WiFi_connecter wc;
void wifi_con_update()
{
  wc.update();
}
#if MQTT_ENABLED
Mqtt_manager mqttm;
void mqtt_update()
{
  mqttm.update();
}
#endif
#include "com/tgesp.h"
tgesp com;
void com_update()
{
  com.update();
}


unsigned long int t_g;


int loop_counter = 0;


void print_info()
{
  Serial.print("T:");Serial.print(t_g++);Serial.print(" V:");Serial.print(TG_VERSION);
  Serial.print("| ");wc.debugg();Serial.println("");
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
  wc.setup();
  #if MQTT_ENABLED
  mqttm.setup();
  #endif
  com.setup();
  ts.add(0, wc.update_rate,wifi_con_update);
  #if MQTT_ENABLED
  ts.add(2,mqttm.update_rate,mqtt_update);
  #endif
  ts.add(3,com.update_rate,com_update);
  ts.add(4,1000,print_info);
}


void loop()
{
  ts.update();
}
