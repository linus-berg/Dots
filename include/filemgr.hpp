#ifndef __FILEMGR_H__
#define __FILEMGR_H__
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <set>

class FileManager {
  public:
  FileManager(std::string home, const char* cfg_path) {
    this->USR_HOME = home;
    this->ParseConfig(cfg_path);
  }
  private: 
  std::string USR_HOME;
  std::map<std::string, std::string> cfg_;
  std::set<boost::filesystem::path> dots_;
  
  bool ParseConfig(const char *cfg_path); 
  bool PackageDots();
};
#endif
