#ifndef PAVELOCK_H
#define PAVELOCK_H

#include <string>
#include <vector>

struct State
{
  std::string prefix;
  std::string jailsRoot;
  std::string setsRoot;
  std::string username;
};

extern State state;

class Jail
{
public:
  static void create(std::string name);
  static void destroy(std::string name);
  static void start(std::string name);
  static void stop(std::string name);
  static void attach(std::string name);
  static void getList(std::vector<std::string>& jails);
  static bool running(std::string name);
  static void updateResolv(std::string name);
};

class Account
{
public:
  static void login(std::string username, std::string password);
  static void registration(std::string username, std::string password, std::string firstName, std::string lastName);
  static std::string sha1(std::string input);
};

class Input
{
public:
  static std::string getText(std::string prompt);
  static std::string getAlphaNumericText(std::string prompt);
  static std::string getPassword(std::string prompt);
  static int getChoice(std::string prompt, std::vector<std::string>& choices, bool allowExit = false);
  static void clear();
  static void notice(std::string prompt);
  static void setBanner(std::string banner);

private:
  static std::string banner;
};

class Process
{
public:
  static std::string execute(std::string command);
  static void run(std::string command, bool silent = true);
};

class Environment
{
public:
  static void setup(std::string path);
};

class Network
{
public:
  static void setup();
};

class Exception : public std::exception
{
public:
  Exception(std::string message);
  virtual ~Exception() throw();
  const char* what() const throw();

private:
  std::string message;
};

#endif
