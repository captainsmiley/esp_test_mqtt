#include <Arduino.h>
#include <TickerScheduler.h>

#include <ESP8266WiFi.h>

#include "conf.h"

#include "sysm/WiFi_connect.h"
#include "sysm/signals.h"
#include "sysm/OTA.h"
#include "app/test_mqtt.h"

#include "com/udp_debug.h"

#if COMMANDS_ENABLED
#include "com/tcp_server.h"
#endif
#if MQTT_ENABLED
#include "sysm/mqtt.h"
#endif


TickerScheduler ts(6);

Signals sig;

//UdpDebug udp;

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

Command c1;
Command c2;

Command * commands1[] =
{
  &c1,
  &c2
};


Command c3;
Command c4;

Command * commands2[] =
{
  &c3,
  &c4
};

Command * tot[] =
{
  commands1,
  commands2,
};

TcpServer com(sig,commands, 2);

void com_update()
{
  com.update();
}

void udp_update()
{
  udp.update();
  //com.cmds.udp.update();
}
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


  Serial.println("");
  String t = String(millis());
  String s = "T: " + t + " V:"+TG_VERSION + " | Free flash: " + ESP.getFreeSketchSpace() +
  " | Free heap:" + ESP.getFreeHeap() + "\n";
  //udp.out(s);

  Serial.print("T:");Serial.print(millis());Serial.print(" V:");Serial.print(TG_VERSION);
  Serial.print("| ");wc.debugg();
  Serial.print("| ");com.debugg();
  Serial.print("| Free flash:");Serial.print(ESP.getFreeSketchSpace());
  Serial.print(" Free heap:");Serial.print(ESP.getFreeHeap());

  String s_var = "{ \"Time\" : " +t+"}";
  udp.out(s_var);
  String tmp =sig.getJsonSignals() ;
  udp.out(tmp);



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
  sig.setup();
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

  udp.setup();


  ts.add(0,ota.update_rate,ota_update);
  ts.add(4, wc.update_rate,wifi_con_update);
  ts.add(5, msg_s.update_rate,msg_sender_update);

  ts.add(1,1000,print_info);
  ts.add(2,udp.update_rate,udp_update);



}


void loop()
{
  ts.update();
}
