//
// Created by mendez on 5/2/23.
//

#ifndef PROYECTO2_UTILS_H
#define PROYECTO2_UTILS_H

#define KEY 2L
#define BUFFER_SIZE 128
#define MAX_KEY (1L << 56)

/*
 * Devuelve el secreto encriptado utilizando la llave del algoritmo.
 *
 * cipher_text: localización donde se guardará el secreto encriptado.
 * returns: longitud del texto encriptado.
 */
int get_encrypted_secret(unsigned char* cipher_text);

/*
 * Verifica si una cadena de texto contiene otra cadena de texto
 *
 * str: cadena donde se buscará la subcadena
 */
bool contains(const unsigned char *str, const unsigned char *sub_str);

/*
 * Recibe un texto cifrado y verifica si la llave proveída desencripta el mensaje.
 *
 * Depende de la constante SECRET_SECTION
 *
 * key: llave sin bits de paridad a probar
 * cipher_text: texto que se desea desencriptar
 * cipher_len: longitud del texto a encriptar
 * buffer: buffer temporal donde se alojará el mensaje desencriptado.
 */
bool key_matches(long key, unsigned char* cipher_text, int cipher_len, unsigned char *buffer);

#endif //PROYECTO2_UTILS_H
