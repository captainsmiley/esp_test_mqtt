/*
 * tgesp.h
 *
 *  Created on: Sep 12, 2016
 *      Author: captainsmiley
 */

#ifndef SRC_TGESP_H_
#define SRC_TGESP_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
//#include <WiFiUdp.h>
#define SERIAL_BUFF_READ_SIZE 10
#define TGESP_UPDATE_RATE_MS 100
#define NR_OF_COMMANDS 2

#include "com/commands.h"
#include "sysm/signals.h"
#include "com/command.h"
//class Commands;

class TcpServer {
public:
	TcpServer(Signals & sig, Command **commands, uint16_t nr_of_commands);
	void setup();
	void update();
	void debugg();
	void stop();
	const static uint8_t update_rate = TGESP_UPDATE_RATE_MS;
    void read_client(WiFiClient & client);
    void listen_for_clients();
    void handle_command(const char *);
		void new_handle_command(const char *);
    void handle_get(String &);
    void readSerial();
    void send_udp();
    void read_udp();
    void handle_http_request(String &);
    const char * cmp_input(const char * input, const char * cmp);
	virtual ~TcpServer();
    Commands cmds;
    void output(const char *);
private:

unsigned long int TimeSinceClientConnect();
	unsigned long int client_connect_time;
	enum SERVER_STATE {WAITING,HANDLING_CLIENT,STOPED};
	SERVER_STATE server_state;
	Signals & sig;
    void send_response();
    static char serial_buff[];
    static size_t serial_buff_pos;
    WiFiServer server;
    bool client_connected;
    WiFiClient connected_client;
    String response_content;
    bool serial_dbg;
    static char packetBuffer[];
		Command ** c_a;
		uint16_t nr_of_c;


};

#endif /* SRC_TGESP_H_ */
