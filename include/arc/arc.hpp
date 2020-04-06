#ifndef __ARC_H__
#define __ARC_H__
#include <encrypt/encrypt.hpp>
#include <archive.h>
#include <archive_entry.h>
#include <boost/filesystem.hpp>
#include <iostream>

typedef struct archive archive;
typedef struct archive_entry entry;


class Meta {
  public:
  std::string name;
  boost::filesystem::fstream out;
  Crypto *crypto;
};

class Archive {
  public: 
  static const int BUFFER_SIZE = 8192;
  Archive(std::string name) {
    /* A 256 bit key */
    std::string key = "01234567890123456789012345678901";
    /* A 128 bit IV */
    std::string iv = "0123456789012345";

    this->meta_.crypto = new Crypto(key, iv);  
    this->archive_name_ = name; 
    this->meta_.name = name;
    this->Init();
  }

  void AddFile(boost::filesystem::path p);
  void Close();
  
  private:
  archive *arc_;  
  entry *entry_;
  char buffer_[BUFFER_SIZE];
  Crypto *cry;

  std::string archive_name_;
  
  /* Meta info for archive */
  Meta meta_;

  void Init();
};

#endif
