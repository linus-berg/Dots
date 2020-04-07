#include <encrypt/encrypt.hpp>

/* Takes arbitrary data input, outputs encrypted into out */
int Crypto::Encrypt(const void *data, int n, void *out) {
  /* Copy input data into unsigned char buffer before encrypting */
  unsigned char buffer[n];
  memcpy(buffer, data, n);
  int crypted_len;
  EVP_EncryptUpdate(this->ctx_, (unsigned char *)out, &crypted_len, buffer, n);
  return crypted_len;
}

/* Puts padding into out */
int Crypto::End(void *out) {
  int len;
  EVP_EncryptFinal_ex(this->ctx_, static_cast<unsigned char *>(out), &len);
  return len;
}

void Crypto::Setup(const char *passphrase) {
  this->ctx_ = EVP_CIPHER_CTX_new();
  /* (48B)384-Digest (sha384)
   * (32B)256-bit key
   * (16B)128-bit salt
   * */
  unsigned char derived[48];
  unsigned char key[32];
  unsigned char iv[16];
  RAND_bytes(this->salt_, 8);
  
  PKCS5_PBKDF2_HMAC(passphrase, strlen(passphrase), this->salt_, 8, 
                    2048, EVP_sha384(), 48, derived);

  /* Use the 32 bytes as a Key and IV */
  memcpy(key, derived, 32);
  memcpy(iv, derived + 32, 16);
  /* Initialise based on key and iv derived from password and salt. */
  EVP_EncryptInit_ex(this->ctx_, EVP_aes_256_cbc(), NULL, key, iv);

  printf("Key: ");
  for (size_t i=0; i<32; ++i)
      printf("%02x", key[i]);
  printf("\n");

  printf("IV:  ");
  for (size_t i=0; i<16; ++i)
      printf("%02x", iv[i]);
  printf("\n");
  
  printf("Salt: ");
  for (size_t i=0; i<8; ++i)
      printf("%02x", this->salt_[i]);
  printf("\n");

}
