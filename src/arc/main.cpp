#include <arc/arc.hpp>

/* Archive callbacks, encrypt before writing */
int open_cb(struct archive *a, void *ln) {
  /* Local cast of ln */
  Links *loc = static_cast<Links *>(ln);
  loc->out.open(loc->archive_name, boost::filesystem::fstream::out);
  if	(loc->out.is_open()) {
    loc->out.write("Salted__", 8);
    loc->out.write(loc->crypto.GetSalt(), 8);
    return (ARCHIVE_OK);
  } else {
    return (ARCHIVE_FATAL);
  }
}

la_ssize_t write_cb(struct archive *a, void *ln, const void *buff, size_t n) {
  Links *loc = static_cast<Links *>(ln);
  /* Encrypt data before writing */
  char crypto_buffer[n];
  int enc_length = loc->crypto.Encrypt(buff, n, crypto_buffer);
  loc->out.write(crypto_buffer, enc_length);
  return enc_length;
}

int close_cb(struct archive *a, void *ln) {
  Links *loc = static_cast<Links *>(ln);
  if	(loc->out.is_open()) {
    /* In case padding is needed */
    char pad[1024];
    int pad_length = loc->crypto.End(pad);
    loc->out.write(pad, pad_length);
    loc->out.close();
    delete loc;
  }
  return (0);
}

void Archive::Init(Links *ln) {
  this->arc_ = archive_write_new(); 
  archive_write_add_filter_gzip(this->arc_);
  archive_write_set_format_gnutar(this->arc_);
  archive_write_open(this->arc_, ln, open_cb, write_cb, close_cb);
}

void Archive::AddFile(boost::filesystem::path base, boost::filesystem::path p) {
  this->entry_ = archive_entry_new();
  boost::filesystem::ifstream in;
  in.open(p);
  if (!in.is_open()) {
    std::cerr << p << " not opened. ";
  }

  /* Write File Metadata */
  boost::filesystem::path tar_path = boost::filesystem::relative(p, base);
  archive_entry_set_pathname(this->entry_, tar_path.string().c_str());
  archive_entry_set_size(this->entry_, boost::filesystem::file_size(p));
  archive_entry_set_filetype(this->entry_, S_IFREG);
  archive_entry_set_perm(this->entry_, 0664);
  archive_write_header(this->arc_, this->entry_);
  
  while (in) {
    in.read(this->buffer_, Archive::BUFFER_SIZE);
    int r = in.gcount();
    archive_write_data(this->arc_, this->buffer_, r);
  }
  in.close();
  archive_entry_free(this->entry_);
}

void Archive::Close() {
  archive_write_close(this->arc_);
  archive_write_free(this->arc_);
}

