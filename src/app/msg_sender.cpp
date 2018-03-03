#include "app/msg_sender.h"
#include "com/commands.h"
#include <ESP8266WiFi.h>
MsgSender::MsgSender(Commands * com, Signals & s) : com(com),
sig(s),
msg("")
{


}

MsgSender::~MsgSender(){}

void MsgSender::update()
{
  readSerial();
  if (sig.MsgUpdated())
  {
    com->send_info_to_clients(sig.get_msg().c_str());
    sig.outdate_msg();
  }
}

void MsgSender::readSerial()
{
    if (Serial.available() > 0) {
      char c = Serial.read();
      if(c=='#' && msg != "")
      {
         Serial.print("Serial recived msg: ");Serial.println(msg);
         sig.set_msg(msg);
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
