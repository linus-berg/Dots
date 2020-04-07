#include <encrypt/encrypt.hpp>

/* Takes arbitrary data input, outputs encrypted into out */
int Crypto::Encrypt(const void *data, int n, void *out) {
  /* Copy input data into unsigned char buffer before encrypting */
  unsigned char buffer[n];
  memcpy(buffer, data, n);
  int crypted_len = 0;
  EVP_EncryptUpdate(this->ctx_, static_cast<unsigned char *>(out),
                    &crypted_len, buffer, n);
  return crypted_len;
}

/* Puts padding into out */
int Crypto::End(void *out) {
  int len = 0;
  EVP_EncryptFinal_ex(this->ctx_, static_cast<unsigned char *>(out), &len);
  return len;
}

void Crypto::Setup(const char *passphrase) {
  this->ctx_ = EVP_CIPHER_CTX_new();
  
  EVP_KDF *kdf;
  EVP_KDF_CTX *kctx{NULL};
  OSSL_PARAM params[3];
  OSSL_PARAM *p = params;

  /* (48B)384-Digest (sha384)
   * (32B)256-bit key
   * (16B)128-bit salt
   * */
  unsigned char derived[48];
  unsigned char key[32];
  unsigned char iv[16];
  char digest[] = "sha384";
  char passw[strlen(passphrase)];
  strcpy(passw, passphrase);

  /* Find and allocate a context for the HKDF algorithm */
  if ((kdf = EVP_KDF_fetch(NULL, "PBKDF2", NULL)) == NULL) {
    std::cerr << "Could not locate PBKDF2" << std::endl;
  }

  kctx = EVP_KDF_CTX_new(kdf);
  EVP_KDF_free(kdf);    /* The kctx keeps a reference so this is safe */
  if (kctx == NULL) {
    std::cerr << "kctx is null" << std::endl;
  }
  
  RAND_bytes(this->salt_, 8);

  /* Build up the parameters for the derivation */
  *p++ = OSSL_PARAM_construct_utf8_string("digest", digest, 6);
  *p++ = OSSL_PARAM_construct_octet_string("salt", this->salt_, 8);
  *p++ = OSSL_PARAM_construct_octet_string("pass", passw, strlen(passw));
  *p = OSSL_PARAM_construct_end();

  if (EVP_KDF_CTX_set_params(kctx, params) <= 0) {
    std::cerr << "set params error" << std::endl;
  }

  /* Do the derivation */
  if (EVP_KDF_derive(kctx, derived, sizeof(derived)) <= 0) {
    std::cerr << "derivation error" << std::endl;
  }

  /* Use the 32 bytes as a Key and IV */
  memcpy(key, derived, 32);
  memcpy(iv, derived + 32, 16);
  
  /* Initialise based on key and iv derived from password and salt. */
  EVP_EncryptInit_ex(this->ctx_, EVP_aes_256_cbc(), NULL, key, iv);
  EVP_KDF_CTX_free(kctx);

  printf("Key: ");
  for (size_t i=0; i<32; ++i)
      printf("%02x ", key[i]);
  printf("\n");

  printf("IV:  ");
  for (size_t i=0; i<16; ++i)
      printf("%02x ", iv[i]);
  printf("\n");
  
  printf("Salt: ");
  for (size_t i=0; i<8; ++i)
      printf("%02x ", this->salt_[i]);
  printf("\n");

}
