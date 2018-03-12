#include "pavelock.h"

#include <stdio.h>

std::string Process::execute(std::string command)
{
  FILE* process = NULL;
  char buffer[64] = {0};
  std::string rtn;

  process = popen(command.c_str(), "r");

  if(process == NULL)
  {
    throw Exception("Failed to run command: '" + command + "'");
  }

  while(fgets(buffer, sizeof(buffer), process) != NULL)
  {
    rtn += buffer;
  }

  if(pclose(process) != 0)
  {
    throw Exception("Command returned error: '" + command + "'");
  }

  return rtn;
}

void Process::run(std::string command, bool silent)
{
  if(silent)
  {
    command += " >/dev/null 2>&1";
  }

  if(system(command.c_str()) != 0)
  {
    throw Exception("Failed to execute: " + command);
  }
}
