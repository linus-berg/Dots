#include <arc.hpp>
#include <filemgr.hpp>
#include <iostream>
#include <vector>

int main() {
  std::string USR_HOME(getenv("HOME"));
  
  FileManager fs(USR_HOME, "/Development/IBM/Dots/cfg/.dots");
  /* Base Paths */
  /* dots = dotfiles */
  boost::filesystem::path cfg_file(USR_HOME + "/Development/IBM/Dots/cfg/.dots");
  


  
  /* Create archive with name OUTPUT and password PASSPHRASE */
  return 0;
}

