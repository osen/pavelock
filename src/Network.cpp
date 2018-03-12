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

  Process::run("ifconfig lo1 alias 127.0.1.2 netmask 255.255.255.255");

/*
  try
  {
    Process::run("ipfw delete 50");
  }
  catch(Exception&) { }
*/

  //Process::run("ipfw nat 200 config if bge0");
  Process::run("ipfw nat 200 config if bge0 reset same_ports redirect_port tcp 127.0.1.2:2000-3000 2000-3000");
  Process::run("ipfw add 50 nat 200 all from any to any");
}
