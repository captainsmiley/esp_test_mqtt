#include <Arduino.h>
#include <TickerScheduler.h>

#include <ESP8266WiFi.h>

#include "conf.h"

#include "sysm/WiFi_connect.h"
#include "sysm/signals.h"
#include "sysm/OTA.h"
#include "app/test_mqtt.h"

//#include "com/udp_debug.h"

#if COMMANDS_ENABLED
#include "com/tgesp.h"
#endif
#if MQTT_ENABLED
#include "sysm/mqtt.h"
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
  //Serial.println("dis all");
  //ts.disableAll();
  ota.update();
  //Serial.println("enable all");
  //ts.enableAll();
}
#if MQTT_ENABLED
Mqtt_manager mqttm;
void mqtt_update()
{
  mqttm.update();
}
#endif

#if COMMANDS_ENABLED
tgesp com(sig);

void com_update()
{
  com.update();
}
/*
void udp_update()
{
//  com.cmds.udp.update();
} */
#endif


#include "app/msg_sender.h"
MsgSender msg_s(&(com.cmds),sig);
void msg_sender_update()
{
  msg_s.update();
}



unsigned long int t_g;


int loop_counter = 0;


void print_info()
{
  Serial.print("T:");Serial.print(millis());Serial.print(" V:");Serial.print(TG_VERSION);
  Serial.print("| ");wc.debugg();
  Serial.print("| ");com.debugg();
  Serial.print("| Free flash:");Serial.print(ESP.getFreeSketchSpace());
  Serial.print(" Free heap:");Serial.print(ESP.getFreeHeap());

  Serial.println("");
  String s = "T: " + String(millis()) + "\n";
  //com.cmds.udp.Out(s);

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
  ota.setup();
  wc.setup();
  #if MQTT_ENABLED
  mqttm.setup();
  ts.add(2,mqttm.update_rate,mqtt_update);
  #endif
  #if COMMANDS_ENABLED
  com.setup();
  ts.add(3,com.update_rate,com_update);
  #endif


  msg_s.setup();

  //com.cmds.udp.setup();


  ts.add(0,ota.update_rate,ota_update);
  ts.add(1, wc.update_rate,wifi_con_update);
  ts.add(2, msg_s.update_rate,msg_sender_update);

  ts.add(4,1000,print_info);
  //ts.add(5,com.cmds.udp.update_rate,udp_update);



}


void loop()
{
  ts.update();
}
