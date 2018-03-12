#include "pavelock.h"

#include <termios.h>
#include <unistd.h>

#include <iostream>

std::string Input::banner;

void Input::setBanner(std::string banner)
{
  Input::banner = banner;
}

std::string Input::getText(std::string prompt)
{
  std::cout << prompt;
  std::string rtn;
  std::getline(std::cin, rtn);

  return rtn;
}

std::string Input::getAlphaNumericText(std::string prompt)
{
  std::string text = getText(prompt);
  std::string whitelist = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  std::string rtn;

  for(size_t i = 0; i < text.length(); i++)
  {
    for(size_t w = 0; w < whitelist.length(); w++)
    {
      if(text.at(i) == whitelist.at(w))
      {
        rtn += whitelist.at(w);
        break;
      }
    }
  }

  return rtn;
}

std::string Input::getPassword(std::string prompt)
{
  std::cout << prompt;
  termios oldt = {0};
  tcgetattr(STDIN_FILENO, &oldt);
  termios newt = oldt;
  newt.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  std::string rtn;
  std::getline(std::cin, rtn);

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  std::cout << std::endl;

  return rtn;
}

int Input::getChoice(std::string prompt, std::vector<std::string>& choices, bool allowExit)
{
  while(true)
  {
    clear();
    std::cout << prompt << std::endl;
    std::cout << std::endl;

    char c = '1';

    for(size_t i = 0; i < choices.size(); i++)
    {
      std::cout << c << ") " << choices.at(i) << std::endl;
      c++;
    }

    if(allowExit)
    {
      std::cout << std::endl << "0) Exit" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Choice: ";

    std::string choice;
    std::getline(std::cin, choice);

    if(allowExit)
    {
      if(choice == "0") return 0;
    }

    for(size_t i = 0; i < choices.size(); i++)
    {
      if(atoi(choice.c_str()) - 1 == i)
      {
        clear();
        return i + 1;
      }
    }

    notice("Invalid choice");
  }
}

void Input::clear()
{
  system("clear");
  std::cout << banner << std::endl;
}

void Input::notice(std::string prompt)
{
  std::cout << std::endl;
  std::cout << prompt << std::endl;
  sleep(2);
  clear();
}
