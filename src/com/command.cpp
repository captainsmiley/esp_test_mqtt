#include "com/command.h"
#include "com/udp_debug.h"


Command::Command(const char* s, CommandHandler h) :
m_h(h),
m_name(String(s)),
next(nullptr)
{

}
Command::Command(String & s, CommandHandler h) :
m_h(h),
m_name(s),
next(nullptr)
{

}

Command::Command() :
m_h(def_h),
m_name("def"),
next(nullptr)
{

}

void Command::def_h(WiFiClient & c, const char * p)
{
  udp.out("def h \n");
}

void Command::run(WiFiClient & c,const char* parameter)
{
  m_h(c,parameter);
}
