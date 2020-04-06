#include <arc/arc.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/config.hpp>
#include <iostream>
#include <set>
#include <vector>

int main() {
  /* iterate recursively */
  std::string USR_HOME = getenv ("HOME");
  
  std::set<boost::filesystem::path> dots;
  boost::filesystem::recursive_directory_iterator end_itr;
  boost::filesystem::ifstream config;
  
  boost::filesystem::path p(USR_HOME + "/Development/IBM/Dots/build");
  std::map<std::string, std::string> cfg;
  cfg["OUTPUT"] = "dots.encrypted";
  std::string line; 
  
  config.open(p / ".dot");
  while(getline(config, line)) {
    if (line[0] == '+') {
      std::vector<std::string> split;
      std::string lin2 = line.substr(1, line.length());
      boost::algorithm::split(split, lin2, boost::is_any_of("="));
      std::cout << split[0] << " = " << split[1] << std::endl;
      cfg[split[0]] = split[1];
    } else {
      boost::filesystem::path dot(p / line);
      if (boost::filesystem::exists(dot)) {
        dots.insert(dot); 
      } else {
        std::cerr << dot << " DOES NOT EXIST." << std::endl;
      }
    }
  }

  Archive arc(cfg["OUTPUT"]);

  for (std::set<boost::filesystem::path>::iterator it = dots.begin(); it != dots.end(); ++it) {
    boost::filesystem::path c = *it;
    for (boost::filesystem::recursive_directory_iterator it(c); it != end_itr; ++it) {
      if (boost::filesystem::is_regular_file(*it)) {
        boost::filesystem::path file = it->path();
        arc.AddFile(file);
        std::cout << "Encrypting: " << file << std::endl;
      }
    }
  }
  arc.Close();
  return 0;
}

