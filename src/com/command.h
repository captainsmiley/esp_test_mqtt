
#include <Arduino.h>
#include <ESP8266WiFi.h>

typedef void (*CommandHandler)(WiFiClient &);
class Command {
public:
  Command(String  & s, CommandHandler h);
  Command(const char * s, CommandHandler h);
  Command();
  void run(WiFiClient &);
  ~Command(){};
  String m_name;
private:
  CommandHandler m_h;
  static void def_h(WiFiClient & c);


};
