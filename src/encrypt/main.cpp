#include <encrypt/encrypt.hpp>

int Crypto::Encrypt(unsigned char *data, int len, unsigned char *out) {
  int crypted_len;
  EVP_EncryptUpdate(this->ctx_, out, &crypted_len, data, len);
  return crypted_len;
}

int Crypto::End(unsigned char *out) {
  int len = 0;
  EVP_EncryptFinal_ex(this->ctx_, out, &len);
  return len;
}


