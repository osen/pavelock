#include "pavelock.h"

#include <sys/signal.h>

#include <iostream>

State state;

void attach_jail()
{
  std::vector<std::string> jails;

  Jail::getList(jails);

  for(size_t i = 0; i < jails.size(); i++)
  {
    if(!Jail::running(jails.at(i)))
    {
      jails.erase(jails.begin() + i);
      i--;
    }
  }

  while(true)
  {
    int choice = Input::getChoice("Which Jail would you like to attach to?", jails, true);

    if(choice == 0) return;

    try
    {
      Jail::attach(jails.at(choice - 1));
    }
    catch(Exception&)
    {
      Input::notice("Failed to attach to jail: " + jails.at(choice - 1));
    }

    return;
  }
}

void start_stop_jail()
{
  std::vector<std::string> jails;

  Jail::getList(jails);
  std::vector<std::string> jailsStatus = jails;

  for(size_t i = 0; i < jailsStatus.size(); i++)
  {
    if(Jail::running(jailsStatus.at(i)))
    {
      jailsStatus.at(i) += " [running]";
    }
    else
    {
      jailsStatus.at(i) += " [stopped]";
    }
  }

  while(true)
  {
    int choice = Input::getChoice("Which Jail would you like to start/stop?", jailsStatus, true);

    if(choice == 0) return;

    if(Jail::running(jails.at(choice - 1)))
    {
      try
      {
        Jail::stop(jails.at(choice - 1));
      }
      catch(Exception&)
      {
        Input::notice("Failed to stop jail: " + jails.at(choice - 1));
      }
    }
    else
    {
      try
      {
        Jail::start(jails.at(choice - 1));
      }
      catch(Exception&)
      {
        Input::notice("Failed to start jail: " + jails.at(choice - 1));
      }
    }

    return;
  }
}

void delete_jail()
{
  std::vector<std::string> jails;

  Jail::getList(jails);

  while(true)
  {
    int choice = Input::getChoice("Which Jail would you like to delete?", jails, true);

    if(choice == 0) return;

    if(Jail::running(jails.at(choice - 1)))
    {
      Input::notice("The Jail '" + jails.at(choice - 1) + "' is currently running so cannot be deleted");
      return;
    }

    try
    {
      Jail::destroy(jails.at(choice - 1));
    }
    catch(Exception&)
    {
      Input::notice("Failed to delete jail: " + jails.at(choice - 1));
    }

    return;
  }
}

void create_jail()
{
  std::vector<std::string> sets;
  int choice = 0;

  Jail::getSets(sets);

  while(true)
  {
    choice = Input::getChoice("What type of Jail would you like to create?", sets, true);

    if(choice == 0) return;
    break;
  }

  while(true)
  {
    std::cout << "Please enter a name for the new Jail" << std::endl << std::endl;
    std::string name = Input::getAlphaNumericText("Name: ");

    try
    {
      Jail::create(name, sets.at(choice - 1));
      Input::notice("Jail created successfully: '" + name + "'");
    }
    catch(Exception& e)
    {
      Input::notice(std::string("Failed to create Jail: ") + e.what());
    }

    return;
  }
}

void menu()
{
  std::vector<std::string> options;
  options.push_back("Attach to running Jail");
  options.push_back("Start/stop existing Jails");
  options.push_back("Create new Jail");
  options.push_back("Delete existing Jail");
  options.push_back("Modify account information");

  while(true)
  {
    int choice = Input::getChoice("Welcome, what would you like to do?", options, true);

    if(choice == 0) return;
    else if(choice == 1) { attach_jail(); }
    else if(choice == 2) { start_stop_jail(); }
    else if(choice == 3) { create_jail(); }
    else if(choice == 4) { delete_jail(); }
  }
}

void registration()
{
  while(true)
  {
    std::cout << "Please enter your new account details" << std::endl << std::endl;
    std::string username = Input::getAlphaNumericText("Username: ");
    std::string firstName = Input::getAlphaNumericText("First Name: ");
    std::string lastName = Input::getAlphaNumericText("Last Name: ");
    std::string password = Input::getPassword("Password: ");
    std::string confirmPassword = Input::getPassword("Confirm Password: ");

    try
    {
      if(password != confirmPassword)
      {
        throw Exception("Passwords do not match");
      }

      Account::registration(username, password, firstName, lastName);
      Input::notice("Registration successful. You may now log in with these details");
      return;
    }
    catch(Exception& e)
    {
      Input::notice(std::string("Failed to register account: ") + e.what());
    }
  }
}

void login()
{
  while(true)
  {
    std::cout << "Please enter your login details" << std::endl << std::endl;
    std::string username = Input::getAlphaNumericText("Username: ");
    std::string password = Input::getPassword("Password: ");

    try
    {
      Account::login(username, password);
      menu();
      return;
    }
    catch(Exception&)
    {
      Input::notice("Invalid login details");
    }
  }
}

void login_or_register()
{
  std::vector<std::string> options;
  options.push_back("Login");
  options.push_back("Register");

  while(true)
  {
    int choice = Input::getChoice("Would you like to login or register?", options, true);

    if(choice == 0) return;
    else if(choice == 1) { login(); return; }
    else if(choice == 2) { registration(); }
  }
}

void sigstop(int p) { }

int main(int argc, char* argv[])
{
  signal(SIGTSTP, &sigstop);

  std::string banner;
  banner += "*****************************************************\n";
  banner += "*    ___  powered by FreeBSD  _               _     *\n";
  banner += "*   / _ \\  __ _ __   __  ___ | |  ___    ___ | | __ *\n";
  banner += "*  / /_)/ / _` |\\ \\ / / / _ \\| | / _ \\  / __|| |/ / *\n";
  banner += "* / ___/ | (_| | \\ V / |  __/| || (_) || (__ |   <  *\n";
  banner += "* \\/      \\__,_|  \\_/   \\___||_| \\___/  \\___||_|\\_\\ *\n";
  banner += "*        Department of Creative Technology          *\n";
  banner += "*****************************************************\n";

  Input::setBanner(banner);

  Input::clear();

  Environment::setup(argv[0]);
  Network::setup();

  login_or_register();

  return 0;
}
