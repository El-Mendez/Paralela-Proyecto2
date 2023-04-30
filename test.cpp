#include <openssl/evp.h>
#include <cstring>
#include <iostream>
#include "crypto.h"

#define KEY 1L
#define BUFFER_SIZE 128

unsigned char SECRET_MSG[BUFFER_SIZE] = "abcdefghjiklmnopqrstuvwxyz";
unsigned char SECRET_SECTION[BUFFER_SIZE] = "fghjikl";

int get_encrypted_secret(unsigned char* cipher_text) {
    long key = prepare_key_parity(KEY);
    int input_len = strlen((char *)SECRET_MSG);

    return encrypt(key, SECRET_MSG, cipher_text, input_len);
}

bool contains(const unsigned char *str, const unsigned char *sub_str) {
    return strstr((char *)str, (char *)sub_str) != nullptr;
}

bool key_matches(long key, unsigned char* cipher_text, int cipher_len, unsigned char *buffer) {
    int len = decrypt(prepare_key_parity(key), cipher_text, buffer, cipher_len);
    buffer[len] = 0;

    return contains(buffer, SECRET_SECTION);
}

int main() {
    init_cipher();

    unsigned char cipher_text[BUFFER_SIZE];
    unsigned char buffer[BUFFER_SIZE];

    int cipher_len = get_encrypted_secret(cipher_text);
    cipher_text[cipher_len] = 0;

    if (key_matches(2L, cipher_text, cipher_len, buffer)) {
        printf("KEY MATCHES\n");
    }

    cleanup_cipher();
}
