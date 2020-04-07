#ifndef __EN_H__
#define __EN_H__
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/kdf.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/params.h>
#include <vector>

class Crypto {
private:
  EVP_CIPHER_CTX *ctx_;
  unsigned char salt_[8];

public:
  Crypto(const char *passphrase) {
    this->Setup(passphrase);
  }

  /* End and get padding */
  int End(void *out);
  
  /* Free Memory */
  ~Crypto() {
    EVP_CIPHER_CTX_free(this->ctx_);
  }
  
  const char * GetSalt() {
    return reinterpret_cast<const char *>(this->salt_);
  }

  int Encrypt(const void *data, int len, void *out);
  void Setup(const char *passphrase);
};

#endif
