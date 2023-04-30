#include <openssl/provider.h>
#include "crypto.h"

EVP_CIPHER_CTX *ctx;

void init_cipher() {
    OSSL_PROVIDER_load(nullptr, "legacy");
    ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_des_ecb(), nullptr, nullptr, nullptr);
}

void cleanup_cipher() {
    EVP_CIPHER_CTX_free(ctx);
}

int encrypt(long key, unsigned char *plain_text, unsigned char *cipher_text, int amount) {
    int cipher_len, temp;
    EVP_EncryptInit_ex(ctx, nullptr, nullptr, (unsigned char*)&key, nullptr);

    EVP_EncryptUpdate(ctx, cipher_text, &temp, plain_text, amount);
    cipher_len = temp;

    EVP_EncryptFinal_ex(ctx, cipher_text + temp, &temp);
    cipher_len += temp;

    return cipher_len;
}

int decrypt(long key, unsigned char *cipher_text, unsigned char *plain_text, int amount) {
    int plain_len, temp;
    EVP_DecryptInit_ex(ctx, nullptr, nullptr, (unsigned char*)&key, nullptr);

    EVP_DecryptUpdate(ctx, plain_text, &temp, cipher_text, amount);
    plain_len = temp;

    EVP_DecryptFinal(ctx, plain_text + temp, &temp);
    plain_len += temp;

    return plain_len;
}

long prepare_key_parity(long key) {
    // shift to add the parity bits
    long k = 0;
    for(int i = 0; i < 8; ++i){
        key <<= 1;
        k += (key & (0xFE << i*8));
    }
    return k;
}
