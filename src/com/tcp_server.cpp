/*
 * TcpServer.cpp
 *
 *  Created on: Sep 12, 2016
 *      Author: captainsmiley
 */

#include "com/tcp_server.h"
#include "com/commands.h"
#include "com/udp_debug.h"
//#include "../lib/StandardCplusplus/StandardCplusplus.h"
//include "../lib/StandardCplusplus/vector"
//#include <vector>

char TcpServer::serial_buff[SERIAL_BUFF_READ_SIZE];
size_t TcpServer::serial_buff_pos = 0;

void tgtest(WiFiClient & c,const char *)
{

  Serial.println("tgtest");
  udp.out("tgtest");
}

TcpServer::TcpServer(Signals & s) : server(80),
    client_connected(false),
	serial_dbg(true),
    server_state(WAITING),
    sig(s),
    lines({""}),
    line("")
{
  test_command = Command("tgtest",tgtest);
  commands.add(&test_command);
}

void TcpServer::setup()
{
  Serial.println("Setup TcpServer");
    server.begin();
}

void TcpServer::update()
{
  switch(server_state)
  {
    case WAITING:
    server_state = HANDLING_CLIENT;
    listen_for_clients();
    server_state = WAITING;
    break;
    case HANDLING_CLIENT:
    break;
    case STOPED:
    break;
    default:
    Serial.println("err state");
    break;
  }
}

void TcpServer::debugg()
{
}

void TcpServer::stop()
{
	server.stop();
}

void TcpServer::listen_for_clients()
{
    WiFiClient client = server.available();
    if (client) {
      Serial.println("client_connected");
      client_connect_time = millis();
        client_connected = true;
        connected_client = client;
        yield();
        read_client(client);
        //client.stop();
        client_connected = false;
        response_content = "";
        client.flush();
    }
}

void TcpServer::send_udp()
{
	//Serial.print("s udp");
	//*(serial_buff+serial_buff_pos)= '\0';
	//cmds.send_udp(serial_buff,serial_buff_pos,udp);

	//serial_buff_pos = 0;
}

char TcpServer::packetBuffer[255]= {};
void TcpServer::read_udp()
{
	//Serial.print("r udp");
	// if there's data available, read a packet
	  //int packetSize = udp.parsePacket();
	  //if (packetSize) {

	    // read the packet into packetBufffer
	   // int len = udp.read(packetBuffer, 255);
	   // if (len > 0) {
	    //  packetBuffer[len] = 0;
	    //  Serial.print(packetBuffer);
	   // }
	 // }
}

void TcpServer::readSerial()
{
	while (Serial.available() > 0 )
	{
		*(serial_buff+serial_buff_pos) = Serial.read();
		serial_buff_pos++;
		if (serial_buff_pos+1>=SERIAL_BUFF_READ_SIZE) break;
	}
}


const char * TcpServer::cmp_input(const char * input, const char * cmp)
{
    if (*cmp == '\0') {
        if (*input == '\0') return input;
        if (*input == 32) return ++input;
        if (*input == '?') return ++input;
        return nullptr;
    }
    if (*input == '\0' || *input == 32) return nullptr;
    if (*input++ == *cmp++) return  cmp_input(input,cmp);
    return nullptr;
}


void TcpServer::new_handle_command(const char * input)
{
  Command * c = commands.head;

  while (c != nullptr) {
    const char * parameter;
    if((parameter = cmp_input(input,c->m_name.c_str())))
    {
      c->run(connected_client,parameter);
    }

    c = c->next;
  }

}

void TcpServer::handle_command(const char * input)
{
    output("<p>handling command: "); output(input); output("</p>");
    new_handle_command(input);

       const char * parameter;
/*
       if (cmp_input(input,"read_client")) {
           if (serial_dbg) Serial.println("Runs read_client()"); cmds.read_client();}
       if (cmp_input(input,"servo_h")) {
           if (serial_dbg) Serial.println("Runs servo_h()"); cmds.servo_h();}
       if (cmp_input(input,"servo_l")) {
           if (serial_dbg) Serial.println("Runs servo_l()"); cmds.servo_l();}
      if (cmp_input(input,"get_id"))
      {
          if (serial_dbg) Serial.println("Runs get_id()"); output(String(cmds.get_id()).c_str());
      }

       if ((parameter = cmp_input(input,"test_gpio")))
       {
       if (serial_dbg) Serial.print("Runs test_gpio("); if (serial_dbg) Serial.print(parameter); Serial.print(")\n");
       Commands::test_gpio(parameter);
       }

       if ((parameter = cmp_input(input,"test_servo")))
       {
       if (serial_dbg) Serial.print("Runs test_servo("); if (serial_dbg) Serial.print(parameter); Serial.print(")\n");
       cmds.test_servo(parameter);
       }

       if ((parameter = cmp_input(input,"set_servo_h_pos")))
       {
           if (serial_dbg) Serial.print("Runs set_servo_h_pos("); if (serial_dbg) Serial.print(parameter); Serial.print(")\n");
           cmds.set_servo_h_pos(parameter);
       }
       if ((parameter = cmp_input(input,"set_servo_l_pos")))
       {
           if (serial_dbg) Serial.print("Runs set_servo_l_pos("); if (serial_dbg) Serial.print(parameter); Serial.print(")\n");
           cmds.set_servo_l_pos(parameter);
       }
       if ((parameter = cmp_input(input,"set_servo_delay")))
       {
           if (serial_dbg) Serial.print("Runs set_servo_delay("); if (serial_dbg) Serial.print(parameter); Serial.print(")\n");
           cmds.set_servo_delay(parameter);
       }
       if ((parameter = cmp_input(input,"set_client_ip")))
       {
           if (serial_dbg) Serial.print("Runs set_client_ip("); if (serial_dbg) Serial.print(parameter); Serial.print(")\n");
           cmds.set_udp_client_ip(parameter);
       }
       if ((parameter = cmp_input(input,"send_serial")))
       {
           if (serial_dbg) Serial.print("Runs send_serial("); if (serial_dbg) Serial.print(parameter); Serial.print(")\n");
           cmds.send_serial(parameter);
       }
       if ((parameter = cmp_input(input,"send_params_serial")))
       {
           if (serial_dbg) Serial.print("Runs send_serial("); if (serial_dbg) Serial.print(parameter); Serial.print(")\n");
           cmds.send_params_serial(parameter);
       }
       if ((parameter = cmp_input(input,"connect_to_ssid")))
       {
    //	   connectToWifi(parameter,"50044801");
       }
       if ((parameter = cmp_input(input,"wifi_setup")))
       {
    	   cmds.wifi_setup(parameter);
       }
       if ((parameter = cmp_input(input,"controll")))
       {
         if (serial_dbg) Serial.print("Runs controll("); if (serial_dbg) Serial.print(parameter); Serial.print(")\n");
         cmds.controll(parameter);
       }
       if ((parameter = cmp_input(input,"set_id")))
       {
           if (serial_dbg) Serial.print("Runs set_id("); if (serial_dbg) Serial.print(parameter); Serial.print(")\n");
           cmds.set_id(parameter);
       }
       if ((parameter = cmp_input(input,"set_main_sta")))
       {
           if (serial_dbg) {Serial.print("Runs set_main_sta("); Serial.print(parameter); Serial.print(")\n"); }
           cmds.set_main_sta(parameter);
       }
       if ((parameter = cmp_input(input,"set_main_sta_pass")))
       {
           if (serial_dbg) {Serial.print("Runs set_main_sta_pass("); Serial.print(parameter); Serial.print(")\n"); }
           cmds.set_main_sta_pass(parameter);
       }
       if (cmp_input(input,"get_main_sta"))
       {
         if (serial_dbg) Serial.println("Runs get_main_sta()");
         cmds.get_main_sta();
       }
       if (cmp_input(input,"get_main_sta_pass"))
       {
         if (serial_dbg) Serial.println("Runs get_main_sta_pass()");
         cmds.get_main_sta_pass();
       }
       if ((parameter = cmp_input(input,"set_no_msg_timeout_reconnect")))
       {
           if (serial_dbg) {Serial.print("Runs set_no_msg_timeout_reconnect("); Serial.print(parameter); Serial.print(")\n"); }
           cmds.set_no_msg_timeout_reconnect(parameter);
       }
       if (cmp_input(input,"get_main_sta"))
       {
         if (serial_dbg) Serial.println("Runs get_main_sta()");
         cmds.get_main_sta();
       }
       if ((parameter = cmp_input(input,"set_try_sta")))
       {
           if (serial_dbg) {Serial.print("Runs set_try_sta("); Serial.print(parameter); Serial.print(")\n"); }
           cmds.set_try_sta(parameter);
       }
       if (cmp_input(input,"get_try_sta"))
       {
         if (serial_dbg) Serial.println("Runs get_try_sta()");
         cmds.get_try_sta();
       }
       if (cmp_input(input,"get_servo_delay"))
       {
         if (serial_dbg) Serial.println("Runs get_servo_delay()");
         cmds.get_servo_delay();
       }
       if (cmp_input(input,"get_servo_l_pos"))
       {
         if (serial_dbg) Serial.println("Runs get_servo_l_pos()");
         cmds.get_servo_l_pos();
       }
       if (cmp_input(input,"get_servo_h_pos"))
       {
         if (serial_dbg) Serial.println("Runs get_servo_h_pos()");
         cmds.get_servo_h_pos();
       }
       if (cmp_input(input,"run_servo_test"))
       {
         if (serial_dbg) Serial.println("Runs run_servo_test()");
         cmds.run_servo_test();
       }
       */

}

void TcpServer::output(const char * out_put)
{
    if (serial_dbg) Serial.print(out_put);
    if (client_connected)
    {
        response_content += out_put;
    }
}

void TcpServer::handle_get(String & cm)
{
    handle_command(cm.substring(1).c_str());
}
int find_text(String needle, String haystack) {
  int foundpos = -1;
  for (int i = 0; i <= haystack.length() - needle.length(); i++) {
    if (haystack.substring(i,needle.length()+i) == needle) {
      foundpos = i;
    }
  }
  return foundpos;
}
void TcpServer::handle_post()
{
  udp.out("handle post \n");
  for(int i=0;i<nr_of_lines;i++)
  {
    yield();
    udp.out(lines[i]+ "\n");
    Serial.println(lines[i]);
    if(lines[i].substring(0,16) == "Content-Length: ")
    {
      String tmp = lines[i].substring(16);
      udp.out(" Find content length:");
      udp.out(tmp);
    }
  }
    udp.out(body);

}

void TcpServer::handle_http_request()
{
    if (lines[0].substring(0,3) == "GET")
    {
        String cm = lines[0].substring(4,lines[0].indexOf(' ',4));
        handle_get(cm);
    }else if (lines[0].substring(0,4)=="POST") {
      handle_post();
    }
    else
    {
      output("Err bad rq: not valid");
    }
}

void TcpServer::ParsBody()
{
  body = "";
  while(connected_client.available() &&
        body.length()<100)
  {
    char c =connected_client.read();
    body+= c;
  }

}


void TcpServer::read_client(WiFiClient & client)
{
  boolean currentLineIsBlank = true;
  //std::vector<String> lines = {};
  uint8_t curr_line = 0;
  response_content = "";
  nr_of_lines = 0;
  while (client.connected()) {
    yield();
    if (TimeSinceClientConnect() > 10000)
    {
      Serial.println("Err timeout connection");
      udp.out("Err timeout connection\n");
      return;
    }
    if (client.available()) {
      char c = client.read();
      // if you've gotten to the end of the line (received a newline
      // character) and the line is blank, the http request has ended,
      // so you can send a reply
      if (c == '\n' && currentLineIsBlank) {
        //body = client.readString();
        ParsBody();
        handle_http_request();
        client.flush();
        send_response();
        nr_of_lines=0;
        return;
      }
      if (c == '\n') {
        // you're starting a new line
        if (++nr_of_lines>19)
        {
          output("Err bad rq: too many lines\n");
          udp.out("Err bad rq: too many lines\n");
          //client.flush();
          //send_response();
          return;
        }
        //udp.out(lines[curr_line]+"\n");
        lines[curr_line++] = line;
        line = "";
        currentLineIsBlank = true;
      }
      else if (c != '\r') {
        // you've gotten a character on the current line
        currentLineIsBlank = false;
        line += c;
      }
    }
  }
}

unsigned long int TcpServer::TimeSinceClientConnect()
{
  return  millis() - client_connect_time;
}

void TcpServer::send_response()
{
  //Serial.println("sending response");
    // send a standard http response header
    connected_client.print("HTTP/1.1 200 OK\r\n");
    connected_client.print("Content-Type: text/html\r\n");
    connected_client.print("Connection: close\r\n");
    //          client.print("Cache-Control: no-cache\r\n");
    connected_client.print("Content-Length: " + String(response_content.length()) + "\r\n");
    //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
    connected_client.print("\r\n");
    connected_client.print(response_content);
}

TcpServer::~TcpServer() {
}
