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
  boost::filesystem::ifstream cfg_istream;
  
  /* Base Paths */
  /* dots = dotfiles */
  boost::filesystem::path cfg_file(USR_HOME + "/Development/IBM/Dots/cfg/.dots");
  boost::filesystem::path dots_dir(USR_HOME + "/Development/IBM/Dots/build");
  
  
  /* Default config values */
  std::map<std::string, std::string> cfg;
  cfg["OUTPUT"] = "dots.encrypted";
  cfg["PASSPHRASE"] = "";
  cfg["BASE_DIR"] = "";

  /* cfg line buffer... */
  char cfg_line_buffer[1024]; 
  cfg_istream.open(cfg_file);
  /* This could go wrong, I assume byte length < 1024 on each line. */
  while(cfg_istream.getline(cfg_line_buffer, 1024) && cfg_istream.is_open()) {
    if (cfg_line_buffer[0] == '+') {
      char *res = strchr(cfg_line_buffer + 1, '=');
      int sign_pos = res - cfg_line_buffer;
      char opt[512];
      char val[512];

      /* strncpy does not null-terminate */
      strncpy(opt, cfg_line_buffer + 1, sign_pos - 1);
      opt[sign_pos - 1] = '\0';
      strcpy(val, cfg_line_buffer + (sign_pos + 1));
      cfg[opt] = val;
      if (!strcmp(opt, "BASE_DIR")) {
        if (val[0] == '~') {
          dots_dir = USR_HOME;
          dots_dir /= val + 1; 
        } else {
          dots_dir = val;
        }
      }
    } else {
      /* Assumes dot dir was setup previoulsy */
      boost::filesystem::path dot(cfg_line_buffer);
      if (boost::filesystem::exists(dots_dir / dot)) {
        dots.insert(dot); 
      } else {
        std::cerr << dot << " DOES NOT EXIST." << std::endl;
      }
    }
  }
  cfg_istream.close();

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
      arc.AddFile(dots_dir, *dot);
    } else if (boost::filesystem::is_directory(*dot)) {
      boost::filesystem::recursive_directory_iterator it(dots_dir / *dot);
      for (; it != end_itr; ++it) {
        if (boost::filesystem::is_regular_file(*it)) {
          std::cout << "Encrypting: " << *it << std::endl;
          arc.AddFile(dots_dir, *it);
        }
      }
    }
  }
  arc.Close();
  return 0;
}

