#include "app/msg_sender.h"
#include "com/commands.h"
#include <ESP8266WiFi.h>
MsgSender::MsgSender(Commands * com) : com(com),
msg("")
{


}

MsgSender::~MsgSender(){}

void MsgSender::update()
{

    if (Serial.available() > 0) {
      char c = Serial.read();
      if(c=='#' && msg != "")
      {
         Serial.print("Serial recived msg: ");Serial.println(msg);
         com->send_info_to_clients(msg.c_str());
         msg = "";
      }
      else
      {
        msg += c;
      }
    }
    if (msg.length() > 5) msg = "";
}

void MsgSender::setup()
{
  Serial.println("Setup MsgSender");

}
