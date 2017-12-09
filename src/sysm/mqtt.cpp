#include "sysm/mqtt.h"
#include "sysm/WiFi_connect.h"
PubSubClient Mqtt_manager::client(WiFi_connecter::client);


const char* mqtt_server = "192.168.0.102";

long lastMsg = 0;
char msg[50];
int value = 0;

Mqtt_manager::Mqtt_manager()
{

}

void Mqtt_manager::setup() {
  Serial.println("Mqtt setup");
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

Mqtt_manager::f_ptr_callback Mqtt_manager::callbacks[NR_MAX_SUBS] = {};
const char* Mqtt_manager::topics[NR_MAX_SUBS] = {};
uint8_t Mqtt_manager::nr_subs = 0;

void Mqtt_manager::add_sub_topic(const char *topic, f_ptr_callback c)
{
  callbacks[nr_subs] = c;
  topics[nr_subs++] = topic;
}

void Mqtt_manager::callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  for (int i = 0; i<nr_subs; ++i)
  {
    if (strcmp(topic,topics[i])==0)
    {
      callbacks[i](topic,payload,length);
    }
  }




}

void Mqtt_manager::reconnect() {
  // Loop until we're reconnected
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("debug", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
      for (int i = 0; i<nr_subs; ++i)
      {
        client.subscribe(topics[i]);
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again net update");
      // Wait 5 seconds before retrying
      //delay(5000);
    }
  }
}
void Mqtt_manager::update() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
/*
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }*/
}
