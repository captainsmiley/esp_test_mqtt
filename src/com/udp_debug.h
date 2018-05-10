
#ifndef APP_UDP_DEBUG_TTG
#define APP_UDP_DEBUG_TTG
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "sysm/signals.h"
//#include "com/command.h"
#define UDP_DEBUG_UPDATE_RATE_MS 50

#define UDP_BUFF_READ_SIZE 100

class UdpDebug
{
public:
  UdpDebug();
  virtual ~UdpDebug();

  const static uint16_t update_rate = UDP_DEBUG_UPDATE_RATE_MS;
  void update();
  void setup();
  void Start();
  bool out(String & s);
  bool out(const char* s);
  void set_client_ip(const char *p);


private:
  char udp_buff[UDP_BUFF_READ_SIZE];
  size_t udp_buff_pos;
  WiFiUDP udp;
  //static char packetBuffer[];
  IPAddress client_ip;
  unsigned int localPort;
  void send_udp();




};

extern UdpDebug udp;


#endif
