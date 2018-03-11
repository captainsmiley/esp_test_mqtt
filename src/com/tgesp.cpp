/*
 * tgesp.cpp
 *
 *  Created on: Sep 12, 2016
 *      Author: captainsmiley
 */

#include "com/tgesp.h"
#include "com/commands.h"
//#include "../lib/StandardCplusplus/StandardCplusplus.h"
//include "../lib/StandardCplusplus/vector"
//#include <vector>

char tgesp::serial_buff[SERIAL_BUFF_READ_SIZE];
size_t tgesp::serial_buff_pos = 0;

tgesp::tgesp(Signals & s) : server(80),
    client_connected(false),
	serial_dbg(true),
    cmds(this,s),
    server_state(WAITING),
    sig(s)
{
}

void tgesp::setup()
{
  Serial.println("Setup tgesp");
    server.begin();
}

void tgesp::update()
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

void tgesp::debugg()
{
  Serial.print("id: ");Serial.print(cmds.get_id());
}

void tgesp::stop()
{
	server.stop();
}

void tgesp::listen_for_clients()
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

void tgesp::send_udp()
{
	//Serial.print("s udp");
	//*(serial_buff+serial_buff_pos)= '\0';
	cmds.send_udp(serial_buff,serial_buff_pos,udp);

	serial_buff_pos = 0;
}

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
}

void tgesp::readSerial()
{
	while (Serial.available() > 0 )
	{
		*(serial_buff+serial_buff_pos) = Serial.read();
		serial_buff_pos++;
		if (serial_buff_pos+1>=SERIAL_BUFF_READ_SIZE) break;
	}
}

void tgesp::scanWifi() {
    // scan for nearby networks:
    if (serial_dbg) Serial.println("** Scan Networks **");
    byte numSsid = WiFi.scanNetworks();

    // print the list of networks seen:
    if (serial_dbg) Serial.print("SSID List:");
    if (serial_dbg) Serial.println(numSsid);
    // print the network number and name for each network found:
    for (int thisNet = 0; thisNet<numSsid; thisNet++) {
        if (serial_dbg) Serial.print(thisNet);
        if (serial_dbg) Serial.print(") Network: ");
        if (serial_dbg) Serial.println(WiFi.SSID(thisNet));
    }

}

void tgesp::connectToWifi()
{
}




void tgesp::createAP()
{

}

bool tgesp::connectToWifi(const char * ssid_p, const char * pass_p)
  {

 }

const char * tgesp::cmp_input(const char * input, const char * cmp)
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

void tgesp::handle_command(const char * input)
{
    output("<p>handling command: "); output(input); output("</p>");

       const char * parameter;

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
    	   connectToWifi(parameter,"50044801");
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

}

void tgesp::output(const char * out_put)
{
    if (serial_dbg) Serial.print(out_put);
    if (client_connected)
    {
        response_content += out_put;
    }
}

void tgesp::handle_get(String & cm)
{
    handle_command(cm.substring(1).c_str());
}

void tgesp::handle_http_request(String & rq)
{
    if (rq.substring(0,3) == "GET")
    {
        String cm = rq.substring(4,rq.indexOf(' ',4));
        handle_get(cm);
    }
    else
    {
      output("Err bad rq: not valid");
    }
}


void tgesp::read_client(WiFiClient & client)
{
    boolean currentLineIsBlank = true;
    //std::vector<String> lines = {};
    String lines[10] ={};
    String line = {};
    uint8_t curr_line = 0;
    response_content = "";
    uint8_t n_lines =0;
    while (client.connected()) {
      yield();
      if (TimeSinceClientConnect() > 10000)
      {
        Serial.println("Err timeout connection");
        return;
      }
        if (client.available()) {
            char c = client.read();
            // if you've gotten to the end of the line (received a newline
            // character) and the line is blank, the http request has ended,
            // so you can send a reply
            if (c == '\n' && currentLineIsBlank) {
                handle_http_request(lines[0]);
                client.flush();
                send_response();
                break;
            }
            if (c == '\n') {
                // you're starting a new line
                if (++n_lines>9)
                {
                  output("Err bad rq: too many lines\n");
                  //client.flush();
                  //send_response();
                  return;
                }
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

unsigned long int tgesp::TimeSinceClientConnect()
{
  return  millis() - client_connect_time;
}

void tgesp::send_response()
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

tgesp::~tgesp() {
}
