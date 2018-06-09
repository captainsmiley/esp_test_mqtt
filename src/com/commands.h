

#ifndef COMMANDS_H
#define COMMANDS_H
#include "com/command.h"
#include <Arduino.h>



class Commands {


public:
  Commands(){}
  void add(Command *);
  ~Commands(){}
  static Command * head;
private:
  static Command * last;


};


#endif

extern Commands commands;
