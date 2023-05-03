//
// Created by mendez on 4/29/23.
//

#ifndef PROYECTO2_CRYPTO_H
#define PROYECTO2_CRYPTO_H

#include <openssl/evp.h>

/*
 * Inicializa todos los parámetros para el cifrado. Se debe ejecutar antes de cualquier algoritmo de
 * encriptación y desencriptado.
 */
void init_cipher();

/*
 * Libera toda la memoria utilizada por la encriptación y desencriptar. Debe llamarse después de
 * init_cipher. De no ser ejecutada, se generará una fuga de memoria.
 */
void cleanup_cipher();

/*
 * Toma una llave y le coloca los bits de paridad para el algoritmo DES ECB.
 *
 * key: un long de 0 a 54 bits.
 * output: una llave con los bits de paridad de 64 bits.
 */
long prepare_key_parity(long key);

/*
 * Encripta el mensaje.
 *
 * key: la llave con los bits de paridad a encriptar
 * plain_text: texto a encriptar
 * amount: longitud del texto a encriptar
 *
 * cipher_text: ubicación donde se guardará el texto encriptado.
 * returns: longitud del mensaje encriptado
 */
int encrypt(long key, unsigned char *plain_text, unsigned char *cipher_text, int amount);
int decrypt(long key, unsigned char *cipher_text, unsigned char *plain_text, int amount);

#endif //PROYECTO2_CRYPTO_H
