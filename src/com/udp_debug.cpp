
#include "com/udp_debug.h"
#include <ESP8266WiFi.h>


UdpDebug udp;

UdpDebug::UdpDebug() :
udp(WiFiUDP()),
localPort(11000),
udp_buff_pos(0)
{}

UdpDebug::~UdpDebug(){}

void UdpDebug::update()
{

  send_udp();
}

void UdpDebug::setup()
{
  Serial.println("Setup udp_debug");
}

void UdpDebug::Start()
{
  udp.begin(localPort);
  Serial.println("udp begin");
}
void UdpDebug::send_udp()
{
  if (udp_buff_pos == 0) return;
  //Serial.println("Send udp");
	 udp.beginPacket(client_ip, 11000);
	 udp.write(udp_buff,udp_buff_pos);
	 udp.endPacket();
	 udp_buff_pos = 0;
}

//char UdpDebug::serial_buff[UDP_BUFF_READ_SIZE];
//size_t tgesp::serial_buff_pos = 0;

bool UdpDebug::out(const char * s)
{
  String tmp(s);
  return out(tmp);
}
bool UdpDebug::out(String & s)
{
  for(int i=0;i<s.length();i++)
  {
		if (udp_buff_pos+1>=UDP_BUFF_READ_SIZE) return false;
		*(udp_buff+udp_buff_pos) = s[i];
		udp_buff_pos++;
  }
  return true;
}
/*
void UdpDebug::readSerial()
{
	while (Serial.available() > 0 )
	{
		*(serial_buff+serial_buff_pos) = Serial.read();
		serial_buff_pos++;
		if (serial_buff_pos+1>=SERIAL_BUFF_READ_SIZE) break;
	}
} */

/*
char tgesp::packetBuffer[255]= {};
void tgesp::read_udp()
{
	//Serial.print("r udp");
	// if there's data available, read a packet
	  int packetSize = udp.parsePacket();
	  if (packetSize) {

	    // read the packet into packetBufffer
	    int len = udp.read(packetBuffer, 255);
	    if (len > 0) {
	      packetBuffer[len] = 0;
	      Serial.print(packetBuffer);
	    }
	  }
}*/

void UdpDebug::set_client_ip(const char *p)
{
	String p_s(p);
	int index1 = p_s.indexOf('.');
	int index2 = p_s.indexOf('.',index1+1);
	int index3 = p_s.indexOf('.',index2+1);
	if (!index1 || !index2 || !index3){
		Serial.println("warning, bad ip");
		return;
	}
	IPAddress ip(
			p_s.substring(0,index1).toInt(),
			p_s.substring(index1+1,index2).toInt(),
			p_s.substring(index2+1,index3).toInt(),
			p_s.substring(index3+1).toInt()
			);
	client_ip = ip;
}
