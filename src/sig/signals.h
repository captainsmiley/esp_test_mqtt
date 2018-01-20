#ifndef SIG_SIGNALS_TTG
#define SIG_SIGNALS_TTG
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#define SIGNALS_UPDATE_RATE_MS 100

#define ID_ADDR 0x04

class Signals
{
public:
  Signals();
  virtual ~Signals();
  void set_id(uint8_t);
  uint8_t get_id() const;

  const static uint8_t update_rate = SIGNALS_UPDATE_RATE_MS;
  void setup();

};


#endif
