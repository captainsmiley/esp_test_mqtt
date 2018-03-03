#ifndef SIG_SIGNALS_TTG
#define SIG_SIGNALS_TTG
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#define SIGNALS_UPDATE_RATE_MS 100

#define SERVO_H_POS_ADDR 0x00
#define SERVO_L_POS_ADDR 0x01
#define SERVO_DELAY_ADDR 0x02
#define ID_ADDR 0x04

class Signals
{
public:
  Signals();
  virtual ~Signals();
  const static uint8_t update_rate = SIGNALS_UPDATE_RATE_MS;
  void setup();

  void set_id(uint8_t);
  uint8_t get_id() const;

  void set_servo_h_pos(uint8_t);
  uint8_t get_servo_h_pos() const;

  void set_servo_l_pos(uint8_t);
  uint8_t get_servo_l_pos() const;

  void set_servo_delay(uint16_t);
  uint16_t get_servo_delay() const;
};


#endif
