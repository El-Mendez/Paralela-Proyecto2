//
// Created by mendez on 5/8/23.
//

#ifndef PROYECTO2_APPROACH_H
#define PROYECTO2_APPROACH_H

struct approach_result {
    long key;
    int rank;
};

approach_result crack_password(int rank, int size, unsigned char* cipher_text, int cipher_len, unsigned char* buffer);

#endif //PROYECTO2_APPROACH_H
