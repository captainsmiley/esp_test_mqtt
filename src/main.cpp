#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <TickerScheduler.h>

#include "sysm/WiFi_connect.h"
#include "sysm/mqtt.h"


#include "app/test_mqtt.h"

#define TG_VERSION 8

TickerScheduler ts(4);


WiFi_connecter wc;
void wifi_con_update()
{
  wc.update();
}
Mqtt_manager mqttm;
void mqtt_update()
{
  mqttm.update();
}
Test_mqtt test(mqttm);
void test_update()
{
  test.update();
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
  mqttm.setup();
  test.setup();
  ts.add(0, wc.update_rate,wifi_con_update);
  ts.add(1,test.update_rate,test_update);
  ts.add(2,mqttm.update_rate,mqtt_update);
  ts.add(3,1000,print_info);
}


void loop()
{
  ts.update();
}
