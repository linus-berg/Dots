#include <filemgr.hpp>
bool FileManager::PackageDots() {
  Archive arc(this->cfg_["OUTPUT"], this->cfg_["PASSPHRASE"]);

  boost::filesystem::recursive_directory_iterator end_itr;
  std::set<boost::filesystem::path>::iterator dot; 
  for (dot = dots.begin(); dot != this->dots_.end(); ++dot) {
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
}
bool FileManager::ParseConfig(const char* cfg_path) {
  boost::filesystem::path cfg_file(cfg_path);
  boost::filesystem::ifstream cfg_istream;
  
  /* Default config values */
  this->cfg_["OUTPUT"] = "dots.encrypted";
  this->cfg_["PASSPHRASE"] = "";
  this->cfg_["BASE_DIR"] = "";
  
  /* cfg line buffer... */
  char cfg_line_buffer[1024]; 
  cfg_istream.open(cfg_file);
  
  while(cfg_istream.getline(cfg_line_buffer, 1024) && cfg_istream.is_open()) {
    if (cfg_line_buffer[0] == '+') {
      char *res = strchr(cfg_line_buffer + 1, '=');
      int sign_pos = res - cfg_line_buffer;
      char opt[512];
      char val[512];

      /* strncpy does not null-terminate */
      strncpy(opt, cfg_line_buffer + 1, sign_pos - 1);
      strcpy(val, cfg_line_buffer + (sign_pos + 1));
      opt[sign_pos - 1] = '\0';
      this->cfg[opt] = val;
      if (!strcmp(opt, "BASE_DIR")) {
        if (val[0] == '~') {
          this->cfg[opt] = USR_HOME;
          this->cfg[opt] /= val + 1;
        }
      }
    } else {
      /* Assumes dot dir was setup previoulsy */
      boost::filesystem::path dot(cfg_line_buffer);
      if (boost::filesystem::exists(dots_dir / dot)) {
        this->dots_.insert(dot); 
      } else {
        std::cerr << dot << " DOES NOT EXIST." << std::endl;
      }
    }
  }
  cfg_istream.close();
  if (!this->cfg_["PASSPHRASE"].size()) {
    std::cerr << "Please set passphrase in config." << std::endl;
    return false;
  } else if (this->dots_.size() <= 0)  {
    std::cerr << "No files to package." << std::endl;
    return false;
  } else if (!this->cfg_["BASE_DIR"].size()) {
    std::cerr << "Please set base dir in config." << std::endl;
    return false;
  }
}
