#ifndef __ARC_H__
#define __ARC_H__
#include <encrypt/encrypt.hpp>
#include <archive.h>
#include <archive_entry.h>
#include <boost/filesystem.hpp>
#include <iostream>

typedef struct archive archive;
typedef struct archive_entry entry;

/* Links to various obj for libarchive */
struct Links {
  /* Archive name */
  std::string archive_name;

  /* Out file handle */
  boost::filesystem::fstream out;
  
  Crypto crypto;

  Links(std::string name, const char *passphrase): crypto(passphrase), archive_name(name) {}
};

class Archive {
public: 
  static const int BUFFER_SIZE{8192};
  Archive(std::string name, std::string passphrase) {
    /* Meta info for archive */
    Links *ln = new Links(name, passphrase.c_str());
    this->Init(ln);
  }

  void AddFile(boost::filesystem::path base, boost::filesystem::path p);
  void Close();
  
private:
  archive *arc_;  
  entry *entry_;
  
  char buffer_[BUFFER_SIZE];
  std::string archive_name_;
  void Init(Links *links);
};

#endif
