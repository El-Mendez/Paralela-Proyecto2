//
// Created by mendez on 4/29/23.
//

#ifndef PROYECTO2_CRYPTO_H
#define PROYECTO2_CRYPTO_H

#include <openssl/evp.h>

void init_cipher();
void cleanup_cipher();

long prepare_key_parity(long key);
int encrypt(long key, unsigned char *plain_text, unsigned char *cipher_text, int amount);
int decrypt(long key, unsigned char *cipher_text, unsigned char *plain_text, int amount);

#endif //PROYECTO2_CRYPTO_H
