#include "app/test_mqtt.h"
#include <ESP8266WiFi.h>

Test_mqtt::Test_mqtt(Mqtt_manager & m):mqttm(m)
{}

Test_mqtt::~Test_mqtt(){}

const char * tgtest_topic = "tgtest";
void handle_tgtest(const char* t,unsigned char* p, unsigned int len)
{
  Serial.println("handling tgtest");
}
void Test_mqtt::update()
{

}

void Test_mqtt::setup()
{
  Serial.println("Setup test_mqtt");
  mqttm.add_sub_topic(tgtest_topic,&handle_tgtest );

}
