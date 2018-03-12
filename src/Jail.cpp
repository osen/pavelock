#include "pavelock.h"

#include <dirent.h>

bool Jail::running(std::string name)
{
  try
  {
    Process::run("jls -j " + state.username + "_" + name);
    return true;
  }
  catch(Exception&) { }

  return false;
}

void Jail::updateResolv(std::string name)
{
  std::string destDir = state.jailsRoot + "/" + state.username + "/" + name;
  Process::run("cp /etc/resolv.conf " + destDir + "/etc/resolv.conf");
}

void Jail::create(std::string name)
{
  std::string destDir = state.jailsRoot + "/" + state.username + "/" + name;
  Process::run("mkdir " + destDir);
  std::string setPath = state.setsRoot + "/freebsd_11_1_amd64/base.txz";
  Process::run("tar -xpf " + setPath + " -C " + destDir);
}

void Jail::destroy(std::string name)
{
  std::string destDir = state.jailsRoot + "/" + state.username + "/" + name;

  // TODO: Ensure that the jail is no longer running

  try
  {
    Process::run("umount -f" + destDir + "/dev");
  }
  catch(Exception&) { }

  Process::run("chflags -R noschg " + destDir);
  Process::run("rm -r -f " + destDir);
}

void Jail::attach(std::string name)
{
  updateResolv(name);
  Process::run("jexec -l -U root " + state.username + "_" + name, false);
}

void Jail::stop(std::string name)
{
  Process::run("jail -r " + state.username + "_" + name);
  std::string destDir = state.jailsRoot + "/" + state.username + "/" + name;
  Process::run("umount " + destDir + "/dev");
}

void Jail::start(std::string name)
{
  std::string destDir = state.jailsRoot + "/" + state.username + "/" + name;
  std::string ip = "127.0.1.2";

  updateResolv(name);

  Process::run("jail -c name=" +
    state.username + "_" + name +
    " path=" + destDir +
    " mount.devfs host.hostname=" + name +
    ".jail ip4.addr=" + ip +
    " interface=lo1 command=sh /etc/rc");
}

void Jail::getList(std::vector<std::string>& jails)
{
  std::string jailsDir = state.jailsRoot + "/" + state.username;

  DIR* dir = opendir(jailsDir.c_str());
  dirent* ent = NULL;

  while((ent = readdir(dir)) != NULL)
  {
    std::string folder = ent->d_name;

    if(folder.at(0) != '_' && folder.at(0) != '.')
    {
      jails.push_back(folder);
    }
  }

  closedir(dir);
}
