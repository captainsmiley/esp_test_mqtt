#include "sig/signals.h"
#include <ESP8266WiFi.h>
Signals::Signals(){}

Signals::~Signals(){}


void Signals::setup()
{
  Serial.println("Setup signals");

}

void Signals::set_id(uint8_t id)
{
    //Serial.print("Setting id: ");Serial.println(n);
    EEPROM.write(ID_ADDR,id);
    EEPROM.commit();
}

uint8_t Signals::get_id() const
{
  EEPROM.read(ID_ADDR);
}
