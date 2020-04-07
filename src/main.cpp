#include <arc/arc.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/config.hpp>
#include <iostream>
#include <set>
#include <vector>

int main() {
  /* iterate recursively */
  std::string USR_HOME(getenv("HOME"));
  
  std::set<boost::filesystem::path> dots;
  boost::filesystem::recursive_directory_iterator end_itr;
  boost::filesystem::ifstream config_file;
  /* Base Paths */
  boost::filesystem::path config_path(USR_HOME + "/Development/IBM/Dots/build");
  boost::filesystem::path dots_path(USR_HOME + "/Development/IBM/Dots/build");
  
  /* Default config values */
  std::map<std::string, std::string> cfg;
  cfg["OUTPUT"] = "dots.encrypted";
  cfg["PASSPHRASE"] = "";
  
  std::string line; 
  config_file.open(config_path / ".dot");
  while(getline(config_file, line) && config_file.is_open()) {
    if (line[0] == '+') {
      std::vector<std::string> opt;
      std::string substring = line.substr(1, line.length());
      boost::algorithm::split(opt, substring, boost::is_any_of("="));
      cfg[opt[0]] = opt[1];
    } else {
      boost::filesystem::path dot(line);
      if (boost::filesystem::exists(dots_path / dot)) {
        dots.insert(dot); 
      } else {
        std::cerr << dot << " DOES NOT EXIST." << std::endl;
      }
    }
  }
  config_file.close();

  if (!cfg["PASSPHRASE"].size()) {
    std::cerr << "Please set passphrase in config." << std::endl;
    return 1;
  }
  
  if (dots.size() <= 0)  {
    std::cerr << "No files to package." << std::endl;
    return 1;
  } 
  Archive arc(cfg["OUTPUT"], cfg["PASSPHRASE"]);

  for (std::set<boost::filesystem::path>::iterator dot = dots.begin();
       dot != dots.end(); ++dot) {
    if (boost::filesystem::is_regular_file(*dot)) {
      std::cout << "Encrypting: " << *dot << std::endl;
      arc.AddFile(dots_path, *dot);
    } else if (boost::filesystem::is_directory(*dot)) {
      boost::filesystem::recursive_directory_iterator it(dots_path / *dot);
      for (; it != end_itr; ++it) {
        if (boost::filesystem::is_regular_file(*it)) {
          std::cout << "Encrypting: " << *it << std::endl;
          arc.AddFile(dots_path, *it);
        }
      }
    }
  }
  arc.Close();
  return 0;
}

