#include "com/command.h"
#include "com/udp_debug.h"


Command::Command(const char* s, CommandHandler h) :
m_h(h),
m_name(String(s))
{

}
Command::Command(String & s, CommandHandler h) :
m_h(h),
m_name(s)
{

}

Command::Command() :
m_h(def_h),
m_name("def")
{

}

void Command::def_h(WiFiClient & c)
{
  udp.out("def h \n");
}

void Command::run(WiFiClient & c)
{
  m_h(c);
}
