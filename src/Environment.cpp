#include "pavelock.h"

#include <sys/stat.h>

#include <iostream>

void Environment::setup(std::string path)
{
  std::string prefix = Process::execute("cd `dirname \\`which " + path + "\\``; cd ..; pwd | tr -d '\n'");

  std::cout << "Pavelock Prefix: " << prefix << std::endl;
  state.prefix = prefix;
  state.jailsRoot = prefix + "/jail";
  state.setsRoot = prefix + "/share/pavelock/sets";
}

bool Environment::pathExists(std::string path)
{
  struct stat s = {0};

  if(stat(path.c_str(), &s) != 0)
  {
    return false;
  }

  return true;
}
