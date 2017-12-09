#ifndef APP_TEMPLATE_TTG
#define APP_TEMPLATE_TTG
#include <ESP8266WiFi.h>
#define TEMPLATE_UPDATE_RATE_MS 100

class Template
{
public:
  Template();
  virtual ~Template();

  const static uint8_t update_rate = TEMPLATE_UPDATE_RATE_MS;
  void update();
  void setup();


};


#endif
