#include "pavelock.h"
#include "sha1.hpp"

#include <fstream>

void Account::login(std::string username, std::string password)
{
  std::ifstream file(state.jailsRoot + "/" + username + "/_passwd");

  if(!file.is_open())
  {
    throw Exception("Failed to open user passwd file: '" + username + "'");
  }

  std::string hash;
  std::getline(file, hash);

  if(hash != sha1(password))
  {
    throw Exception("Specified hash does not match that stored in the passwd file");
  }

  state.username = username;
}

std::string Account::sha1(std::string input)
{
  SHA1 checksum;
  checksum.update(input);
  std::string rtn = checksum.final();

  return rtn;
}
