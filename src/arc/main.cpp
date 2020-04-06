#include <arc/arc.hpp>

/* Archive callbacks, encrypt before writing */
int open_cb(struct archive *a, void *meta) {
  Meta *data = static_cast<Meta *>(meta);
  data->out.open(data->name, boost::filesystem::fstream::out);
  if	(data->out.is_open())
    return (ARCHIVE_OK);
  else
    return (ARCHIVE_FATAL);
}

la_ssize_t write_cb(struct archive *a, void *meta, const void *buff, size_t n) {
  Meta *data = static_cast<Meta *>(meta);
  /* Encrypt data before writing */
  unsigned char crypto_buffer[n + Archive::BUFFER_SIZE];
  int l = data->crypto->Encrypt((unsigned char *)buff, n, crypto_buffer);
  data->out.write((const char *)crypto_buffer, l);
  return l;
}

int close_cb(struct archive *a, void *meta) {
  Meta *data = static_cast<Meta *>(meta);
  if	(data->out.is_open()) {
    unsigned char pad[Archive::BUFFER_SIZE];
    int l = data->crypto->End(pad);
    data->out.write((const char*) pad, l);
    data->out.close();
  }
  return (0);
}

void Archive::Init() {
  this->arc_ = archive_write_new(); 
  archive_write_add_filter_gzip(this->arc_);
  archive_write_set_format_gnutar(this->arc_);
  archive_write_open(this->arc_, &(this->meta_), open_cb, write_cb, close_cb);
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

