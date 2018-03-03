#include "sysm/signals.h"
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
  return EEPROM.read(ID_ADDR);
}


void Signals::set_servo_delay(uint16_t d)
{
    //Serial.print("Setting id: ");Serial.println(n);
  EEPROM.write(SERVO_DELAY_ADDR, uint8_t( d & 0x00FF) );
  EEPROM.write(SERVO_DELAY_ADDR+1, uint8_t( (d & 0xFF00) >> 8 ));
    EEPROM.commit();
}
uint16_t Signals::get_servo_delay() const
{
    return  (EEPROM.read(SERVO_DELAY_ADDR+1) << 8) | EEPROM.read(SERVO_DELAY_ADDR);
}

void Signals::set_servo_h_pos(uint8_t pos)
{
    //Serial.print("Setting id: ");Serial.println(n);
    EEPROM.write(SERVO_H_POS_ADDR,pos);
    EEPROM.commit();
}
uint8_t Signals::get_servo_h_pos() const
{
   return EEPROM.read(SERVO_H_POS_ADDR);
}


void Signals::set_servo_l_pos(uint8_t pos)
{
    //Serial.print("Setting id: ");Serial.println(n);
    EEPROM.write(SERVO_L_POS_ADDR,pos);
    EEPROM.commit();
}
uint8_t Signals::get_servo_l_pos() const
{
   return EEPROM.read(SERVO_L_POS_ADDR);
}