#include "sysm/signals.h"
#include <ESP8266WiFi.h>
Signals::Signals() :
msg(""),
msg_updated(false),
msg_update_time(millis())
{
  EEPROM.begin(256);
}

Signals::~Signals(){}


void Signals::setup()
{
  Serial.println("Setup signals");
  override_values();
}

void Signals::override_values()
{
  set_no_msg_timeout_reconnect(false);
  set_main_sta_on_flash(false);
  set_id(99);
  set_servo_delay(100);
  String sta = "TeliaGateway9C-97-26-49-11-55";
  set_main_sta(sta);
  String pass = "179A1021DD";
  set_main_sta_pass(pass);

}

void Signals::set_id(uint8_t id)
{
    Serial.print("Setting id: ");Serial.println(id);
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

void Signals::set_main_sta(String &s)
{
  Serial.println("writes main sta: "+s);
  if(s.length() > MAIN_STA_MAX_LEN) return;
  for(int i=0;i<s.length();i++)
  {
    EEPROM.write(i+MAIN_STA_ADDR,s[i]);
  }
  EEPROM.write(MAIN_STA_LEN_ADDR,s.length());
    EEPROM.commit();

}

String Signals::get_main_sta() const
{
  String s = "";
  int n =EEPROM.read(MAIN_STA_LEN_ADDR);
  if(n>MAIN_STA_MAX_LEN)
  {
    n = MAIN_STA_MAX_LEN;
  }
  Serial.println("read len: "+String(n));
  for(int i=0;i<n;++i)
  {
    s+= char(EEPROM.read(i+MAIN_STA_ADDR));
  }
  return s;
}

void Signals::set_no_msg_timeout_reconnect(bool b)
{
  CONF1 conf = NO_MSG_TIMEOUT_RECONNECT;
  set_conf1_bit(conf,b);
}
bool Signals::get_no_msg_timeout_reconnect() const
{
  CONF1 conf = NO_MSG_TIMEOUT_RECONNECT;
  return get_conf1_bit(conf);
}
void Signals::set_main_sta_on_flash(bool b)
{
  CONF1 conf = MAIN_STA_ON_FLASH;
  set_conf1_bit(conf,b);
}
bool Signals::get_main_sta_on_flash() const
{
  CONF1 conf = MAIN_STA_ON_FLASH;
  return get_conf1_bit(conf);
}
void Signals::set_conf1_bit(CONF1 pos, bool value)
{
  uint8_t tmp = EEPROM.read(CONF1_ADDR);
  if(value)
  {
    tmp |= (1<<pos);

  }else {
    tmp &= ~(1<<pos);
  }
  EEPROM.write(CONF1_ADDR,tmp);
  EEPROM.commit();
}
bool Signals::get_conf1_bit(CONF1 pos) const
{
  uint8_t tmp = EEPROM.read(CONF1_ADDR);
  return tmp & (1<<pos);

}


void Signals::set_main_sta_pass(String &s)
{
  Serial.println("writes main sta pass: "+s);
  if(s.length() > MAIN_STA_PASS_MAX_LEN) return;
  for(int i=0;i<s.length();i++)
  {
    EEPROM.write(i+MAIN_STA_PASS_ADDR,s[i]);
  }
  EEPROM.write(MAIN_STA_PASS_LEN_ADDR,s.length());
  EEPROM.commit();

}


String Signals::get_main_sta_pass() const
{
  String s = "";
  int n =EEPROM.read(MAIN_STA_PASS_LEN_ADDR);
  if(n>MAIN_STA_PASS_MAX_LEN)
  {
    n = MAIN_STA_PASS_MAX_LEN;
  }
  Serial.println("read len: "+String(n));
  for(int i=0;i<n;++i)
  {
    s+= char(EEPROM.read(i+MAIN_STA_PASS_ADDR));
  }
  return s;
}

void Signals::set_try_sta(String &s)
{
  Serial.println("writes try sta: "+s);
  if(s.length() > TRY_STA_MAX_LEN) return;
  for(int i=0;i<s.length();i++)
  {
    EEPROM.write(i+TRY_STA_ADDR,s[i]);
  }
  EEPROM.write(TRY_STA_LEN_ADDR,s.length());
    EEPROM.commit();

}

String Signals::get_try_sta() const
{
  String s = "";
  int n =EEPROM.read(TRY_STA_LEN_ADDR);
  if(n>TRY_STA_MAX_LEN)
  {
    n = TRY_STA_MAX_LEN;
  }
  Serial.println("read len: "+String(n));
  for(int i=0;i<n;++i)
  {
    s+= char(EEPROM.read(i+TRY_STA_ADDR));
  }
  return s;
}

void Signals::set_self_sta(String &s)
{
  Serial.println("writes self sta: "+s);
  if(s.length() > SELF_STA_MAX_LEN) return;
  for(int i=0;i<s.length();i++)
  {
    EEPROM.write(i+SELF_STA_ADDR,s[i]);
  }
  EEPROM.write(SELF_STA_LEN_ADDR,s.length());
    EEPROM.commit();

}

String Signals::get_self_sta() const
{
  String s = "";
  int n =EEPROM.read(SELF_STA_LEN_ADDR);
  if(n>SELF_STA_MAX_LEN)
  {
    n = SELF_STA_MAX_LEN;
  }
  Serial.println("read len: "+String(n));
  for(int i=0;i<n;++i)
  {
    s+= char(EEPROM.read(i+SELF_STA_ADDR));
  }
  return s;
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


void Signals::set_msg(String &s)
{
  msg_updated = true;
  msg_update_time = millis();
  msg = s;
}
String Signals::get_msg() const
{
  return msg;
}
void Signals::outdate_msg()
{
  msg_updated = false;
}
bool Signals::MsgUpdated() const
{
  return msg_updated;
}
unsigned long Signals::TimeSinceMsgUpdate() const
{
  return (millis() - msg_update_time);
}
