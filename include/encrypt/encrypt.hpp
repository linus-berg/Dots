#ifndef __EN_H__
#define __EN_H__
#include <iostream>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>


class Crypto {
private:
  EVP_CIPHER_CTX *ctx_;
public:
  Crypto(std::string key, std::string iv) {
    this->ctx_ = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(this->ctx_, EVP_aes_256_cbc(), NULL,
        (unsigned char *)(key.c_str()), (unsigned char *)(iv.c_str()));
  }

  /* End and get padding */
  int End(unsigned char *out);
  
  /* Free Memory */
  ~Crypto() {
    EVP_CIPHER_CTX_free(this->ctx_);
  }

  int Encrypt(unsigned char *data, int len, unsigned char *out);
};

#endif
