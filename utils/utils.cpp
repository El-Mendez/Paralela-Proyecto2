//
// Created by mendez on 5/2/23.
//

#include "utils.h"
#include "../crypto/crypto.h"
#include <cstring>
#include <fstream>


unsigned char SECRET_SECTION[BUFFER_SIZE] = "Esta es una prueba de proyecto 2";

int get_encrypted_secret(const std::string &filename, unsigned char* cipher_text) {
    long key = prepare_key_parity(KEY);
    unsigned char BUFF[BUFFER_SIZE];

    // read from file text
    std::ifstream file;
    file.open(filename, std::ios::in);
    if(file.is_open()) {
        file.read((char *)BUFF, BUFFER_SIZE);
    } else {
        printf("NO SE ENCONTRÓ EL ARCHIVO %s \n", filename.c_str());
    }
    file.close();

    return encrypt(key, BUFF, cipher_text, file.gcount());
}

bool contains(const unsigned char *str, const unsigned char *sub_str) {
    return strstr((char *)str, (char *)sub_str) != nullptr;
}

bool key_matches(long key, unsigned char* cipher_text, int cipher_len, unsigned char *buffer) {
    int len = decrypt(prepare_key_parity(key), cipher_text, buffer, cipher_len);
    buffer[len] = 0;

    return contains(buffer, SECRET_SECTION);
}
