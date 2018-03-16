#include "pavelock.h"

#include <dirent.h>

#include <iostream>

void Jail::copyQemuStatic(std::string set, std::string jailRoot)
{
  std::string arch;

  for(int i = set.length() - 1; i >= 0; i--)
  {
    if(set.at(i) == '_')
    {
      arch = set.substr(i + 1);
      break;
    }
  }

  if(arch != "aarch64" && arch != "arm") return;
  std::string qemuStatic = "/usr/local/bin/qemu-" + arch + "-static";
  if(!Environment::pathExists(qemuStatic)) return;

  Process::run("mkdir -p " + jailRoot + "/usr/local/bin");
  Process::run("cp " + qemuStatic + " " + jailRoot + "/usr/local/bin");
}

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

void Jail::create(std::string name, std::string set)
{
  std::string destDir = state.jailsRoot + "/" + state.username + "/" + name;
  Process::run("mkdir " + destDir);
  std::string setPath = state.setsRoot + "/"+set+"/base.txz";

  std::cout << std::endl << "Extracting Jail base. Please wait." << std::endl;

  Process::run("tar -xpf " + setPath + " -C " + destDir);

  copyQemuStatic(set, state.jailsRoot + "/" + state.username + "/" + name);
}

void Jail::destroy(std::string name)
{
  std::string destDir = state.jailsRoot + "/" + state.username + "/" + name;

  // TODO: Ensure that the jail is no longer running

  try
  {
    Process::run("umount " + destDir + "/dev");
  }
  catch(Exception&) { }

  try
  {
    Process::run("umount -f " + destDir + "/dev");
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

  Process::run("jail -c persist name=" +
    state.username + "_" + name +
    " path=" + destDir +
    " mount.devfs host.hostname=" + name +
    ".jail ip4.addr=" + ip +
    " allow.set_hostname=false" +
    " interface=lo1 command=sh /etc/rc", false);
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

void Jail::getSets(std::vector<std::string>& sets)
{
  std::string setsDir = state.setsRoot;

  DIR* dir = opendir(setsDir.c_str());
  dirent* ent = NULL;

  while((ent = readdir(dir)) != NULL)
  {
    std::string folder = ent->d_name;

    // A bit hacky but all folders ^f(reebsd).
    // Ignore README.txt
    if(folder.at(0) == 'f')
    {
      sets.push_back(folder);
    }
  }

  closedir(dir);
}
