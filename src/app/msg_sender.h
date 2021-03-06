#ifndef APP_MsgSender_TTG
#define APP_MsgSender_TTG
#include <ESP8266WiFi.h>
#include "com/commands.h"
#include "sysm/signals.h"
#define MsgSender_UPDATE_RATE_MS 100

class MsgSender
{
public:
  MsgSender(Commands * c, Signals & s);
  virtual ~MsgSender();

  void readSerial();

  const static uint8_t update_rate = MsgSender_UPDATE_RATE_MS;
  void update();
  void setup();
private:
  Commands * com;
  String msg;
  Signals & sig;


};


#endif
