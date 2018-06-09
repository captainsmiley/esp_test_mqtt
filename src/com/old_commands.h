#ifndef SRC_COMMANDS_H_
#define SRC_COMMANDS_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
//#include <WiFiUdp.h>
#include "sysm/signals.h"
#include "com/udp_debug.h"

class TcpServer;
//#include "Params.h"

#define SERVO_H_POS 100
#define SERVO_L_POS 10
#define SERVO_PIN 4
#define SERVO_DELAY 200

#include <Servo.h>

class Commands {
public:
    static Servo sv;
	Commands(TcpServer *,Signals &sig);
    static void test_gpio(const char * p);

    static void http_get();
    static void read_distance();
    static void read_client();
    static void test_servo(const char *p);
    void send_udp(const char *,size_t n,WiFiUDP & udp);
    void servo_h();
    void servo_l();
    void set_servo_h_pos(const char *p);
    void set_servo_l_pos(const char *p);
    void set_servo_delay(const char *p);
    void set_udp_client_ip(const char *p);
    void send_params_serial(const char *p);
    void send_serial(const char *p);
    void wifi_setup(const char *p);
    void connect(const char *p);
    void set_id(const char *p);
    void set_main_sta(const char *p);
    void set_main_sta_pass(const char *p);
    void set_no_msg_timeout_reconnect(const char *p);
    void get_main_sta();
    void get_main_sta_pass();
    void set_try_sta(const char *p);
    void get_try_sta();
    void get_servo_delay();
    void get_servo_h_pos();
    void get_servo_l_pos();
    void controll(const char *p);
    void send_info_to_clients(const char *p);
    void run_servo_test();
    uint8_t get_id();
	virtual ~Commands();
    //UdpDebug udp;

private:
	//Params pars_params(const char *p);

	IPAddress client_ip;
    uint8_t servo_h_pos;
    uint8_t servo_l_pos;
    TcpServer *esp;
    Signals &sig;

};

#endif /* SRC_TGESP_H_ */
