#include <mpi.h>
#include "crypto/crypto.h"
#include "utils/utils.h"
#include "approaches/approach.h"

int main(int argc, char** argv) {
    // get the encrypted message
    init_cipher();
    unsigned char cipher_text[BUFFER_SIZE];
    unsigned char buffer[BUFFER_SIZE];
    int cipher_len = 0;

    // init mpi
    int size, rank;
    MPI_Init(nullptr, nullptr);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        printf("Ingrese el nombre del archivo con el mensaje a encriptar\n");
        char filename[BUFFER_SIZE];
        scanf("%s", filename);
        cipher_len = get_encrypted_secret(filename,cipher_text);
        printf("Encrypted message: %s\n", cipher_text);
        MPI_Bcast(&cipher_len, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&cipher_text, cipher_len, MPI_CHAR, 0, MPI_COMM_WORLD);
    } else {
        MPI_Bcast(&cipher_len, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&cipher_text, cipher_len, MPI_CHAR, 0, MPI_COMM_WORLD);
    }

    long key = crack_password(rank, size, cipher_text, cipher_len, buffer);

    // imprimir resultados
    if (rank == 0) {
        int len = decrypt(prepare_key_parity(key), cipher_text, buffer, cipher_len);
        buffer[len] = 0;

        printf("\nThe key %ld matched %s", key, buffer);
    }

    MPI_Finalize();
    cleanup_cipher();
}
