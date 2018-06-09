

#include "com/commands.h"

Commands commands;

Command * Commands::head = nullptr;
Command * Commands::last = nullptr;

void Commands::add(Command * c)
{
  if(last != nullptr)
  {
    last->next = c;
    last = c;
  } else
  {
    head = c;
    last = c;
  }
  //last->next = c;
}
