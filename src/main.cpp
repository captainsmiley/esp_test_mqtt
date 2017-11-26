#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <TickerScheduler.h>

#include "sysm/WiFi_connect.h"

#define TG_VERSION 7

#define STA_WIFI_ATTEMPT 1

TickerScheduler ts(4);
WiFi_connecter wc;
unsigned long int t_g;


int loop_counter = 0;


void print_info()
{
  Serial.print("T:");Serial.print(t_g++);Serial.print(" V:");Serial.println(TG_VERSION);
  wc.debugg();
}

void app()
{
}

void wifi_con_update()
{
  wc.update();
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Start of main");
  wc.setup();
  ts.add(0, wc.update_rate,wifi_con_update);
  ts.add(1,1000,print_info);
  ts.add(2,100,app);
}


void loop()
{
  ts.update();
}
