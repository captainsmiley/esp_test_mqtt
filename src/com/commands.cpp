

#include "com/commands.h"
#include <Servo.h>
#include <WiFiUdp.h>
#include "com/tgesp.h"



Commands::Commands(tgesp* e,Signals & s) :
    servo_h_pos(s.get_servo_h_pos()),
    servo_l_pos(s.get_servo_l_pos()),
	client_ip(192,168,0,105),
  sig(s),
	esp(e)
  //udp(UdpDebug())
{

}

Commands::~Commands(){}


void Commands::test_gpio(const char * p)
{
    String p_s(p);
    pinMode(p_s.toInt(),OUTPUT);
    if (p_s.endsWith("l")){

        digitalWrite(p_s.toInt(),LOW);
    }
    if (p_s.endsWith("h")){

        digitalWrite(p_s.toInt(),HIGH);
    }

}

void Commands::set_udp_client_ip(const char *p)
{
  //udp.set_client_ip(p);
  //udp.Start();
  /*
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
  */
}

void Commands::wifi_setup(const char *p)
{
	String p_s(p);
	int index1 = p_s.indexOf('&');
	if (!index1){
		Serial.println("warning, bad input");
		return;
	}
	String ap = p_s.substring(0,index1);


	if(ap=="ap")
	{
	esp->createAP();
	return;
	}
	else
	{
		char b1[30];
		char b2[30];
		ap.toCharArray(b1,30,0);
		p_s.substring(index1+1).toCharArray(b2,30,0);
		Serial.print(b1);Serial.println(b2);
		if (!esp->connectToWifi(b1,b2))
		{
			Serial.println("warning, no connection");
			esp->createAP();
		}
	}

}


void Commands::send_udp(const char * ca,size_t n,WiFiUDP & udp)
{
	 udp.beginPacket(client_ip, 11000);
	 udp.write(ca,n);
	 udp.endPacket();
}
void Commands::connect(const char * str)
{
	String p_s(str);
	int index = p_s.indexOf("&");
	if(!index){
		Serial.println("warning bad input");
		return;
	}
}

void Commands::send_serial(const char * str)
{
	Serial.print(str);
}
void Commands::send_params_serial(const char * str)
{
	Serial.print('&');
	Serial.print(str);
	Serial.print('&');
}

void Commands::read_client() {}
void Commands::read_distance() {}
void Commands::http_get() {
Serial.println("runs http_get");
}
Servo Commands::sv = Servo();
void Commands::test_servo(const char * p)
{
    String p_s(p);
    sv.attach(SERVO_PIN);
    sv.write(p_s.toInt());
}

void Commands::servo_h()
{
    sv.attach(SERVO_PIN);
    sv.write(sig.get_servo_h_pos());
    delay(sig.get_servo_delay());
//    delay( (EEPROM.read(SERVO_DELAY_ADDR+1) << 8) | EEPROM.read(SERVO_DELAY_ADDR));
    sv.detach();
}
void Commands::set_servo_delay(const char *p)
{
  String p_s(p);
  uint16_t s_delay = uint16_t(p_s.toInt());
  sig.set_servo_delay(s_delay);

}



void Commands::servo_l()
{
    sv.attach(SERVO_PIN);
    sv.write(sig.get_servo_l_pos());
    delay(sig.get_servo_delay());
    sv.detach();
}

void Commands::set_servo_h_pos(const char *p)
{
    String p_s(p);
    sig.set_servo_h_pos(p_s.toInt());
}
void Commands::set_servo_l_pos(const char *p)
{
    String p_s(p);
    sig.set_servo_l_pos(p_s.toInt());
}

void Commands::set_id(const char *p)
{
    String p_s(p);
    uint8_t n = p_s.toInt();
    Serial.print("Setting id: ");Serial.println(n);
    sig.set_id(n);
}
void Commands::set_main_sta(const char *p)
{
    String p_s(p);
    sig.set_main_sta(p_s);
}

void Commands::get_main_sta()
{
  esp->output(String("Main sta: "+ sig.get_main_sta()).c_str());
}

void Commands::get_servo_delay()
{
  esp->output("Servo delay: ");
  esp->output(String(sig.get_servo_delay()).c_str());
}
void Commands::get_servo_h_pos()
{
  esp->output("Servo h pos: ");
  esp->output(String(sig.get_servo_h_pos()).c_str());
}
void Commands::get_servo_l_pos()
{
  esp->output("Servo l pos: ");
  esp->output( String(sig.get_servo_l_pos()) .c_str() );
}

void Commands::controll(const char *p)
{
    String p_s(p);
    uint16_t n = p_s.toInt();
    Serial.print("Get ctr: ");Serial.println(n);
    //send_info_to_clients(p);
    //Serial.println(get_id());
    //uint16_t sh = get_id();
    //Serial.println(sh);

    if ( n & (0x0001 << get_id() ) )
    {
      Serial.println("servo_h");
      servo_h();
    } else
    {
      Serial.println("servo_l");
      servo_l();
    }

    sig.set_msg(p_s);
}

extern "C" {
#include<user_interface.h>
}

void Commands::send_info_to_clients(const char *p)
{
  struct station_info *stat_info;
  struct ip_addr *IPaddress;
  IPAddress address;
  // Initialize the client library
  WiFiClient client;

  Serial.println("Starting to send info to clinets");
  stat_info = wifi_softap_get_station_info();
  while (stat_info != NULL)
  {
    IPaddress = &stat_info->ip;
    address = IPaddress->addr;


    client.setNoDelay(true);
    client.setTimeout(110);
    Serial.print("Send to: ");Serial.println(address);
    if (client.connect(address, 80)) {

      Serial.print("connected to ");Serial.println(address);
      // Make a HTTP request:
      client.println("GET /controll?"+String(p)+" HTTP/1.0");
      client.println();
      client.flush();
    }
    else
    {
      Serial.print("Faild to send info to ");Serial.println(address);
  //    baned_clients[banned_count] = address;
   //   banned_count++;
    //  if (banned_count >= 10) banned_count = 0;
    }
    client.stop();
    stat_info = STAILQ_NEXT(stat_info, next);
  }

}

uint8_t Commands::get_id()
{
  return sig.get_id();
}
