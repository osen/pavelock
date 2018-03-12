#include "pavelock.h"

#include <iostream>

void Network::setup()
{
  try
  {
    Process::run("ifconfig lo1");
    return;
  }
  catch(Exception&) { }

  Input::notice("Preparing firewall, you may be disconnected");

  Process::run("ifconfig lo1 create");

/*
  try
  {
    Process::run("ipfw delete 50");
  }
  catch(Exception&) { }
*/

  Process::run("ipfw nat 200 config if bge0");
  Process::run("ipfw add 50 nat 200 all from any to any");
}
