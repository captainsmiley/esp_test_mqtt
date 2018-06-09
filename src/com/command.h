#ifndef COMMAND_H
#define COMMAND_H
#include <Arduino.h>
#include <ESP8266WiFi.h>

typedef void (*CommandHandler)(WiFiClient &,const char *);
class Command {
public:
  Command(String  & s, CommandHandler h);
  Command(const char * s, CommandHandler h);
  Command();
  void run(WiFiClient &, const char * parameter);
  ~Command(){};
  String m_name;
  Command * next;
private:
  CommandHandler m_h;
  static void def_h(WiFiClient & c, const char *);


};

#endif
