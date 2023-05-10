#include <mpi.h>
#include "crypto/crypto.h"
#include "utils/utils.h"
#include "approaches/approach.h"
#include <fstream>

int main(int argc, char** argv) {
    // get the encrypted message
    init_cipher();
    unsigned char cipher_text[BUFFER_SIZE];
    unsigned char buffer[BUFFER_SIZE];
    int cipher_len = 0;

    // init mpi
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double start, end;

    if (rank == 0) {
        printf("Ingrese el nombre del archivo con el mensaje a encriptar\n");
        char filename[BUFFER_SIZE] = "secret.txt";
        //scanf("%s", filename);
        cipher_len = get_encrypted_secret(filename, cipher_text);
        cipher_text[cipher_len] = 0;

        // print cipher_text in hex
        printf("Encrypted Message (in hex): 0x");
        for (int i = 0; i < cipher_len; i++) {
            printf("%02x", cipher_text[i]);
        }
        printf("\n");

        start = MPI_Wtime();

        MPI_Bcast(&cipher_len, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&cipher_text, cipher_len, MPI_CHAR, 0, MPI_COMM_WORLD);
    } else {
        MPI_Bcast(&cipher_len, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&cipher_text, cipher_len, MPI_CHAR, 0, MPI_COMM_WORLD);
    }
    long key = crack_password(rank, size, cipher_text, cipher_len, buffer);
    end = MPI_Wtime();
    // imprimir resultados
    if (rank == 0) {
        int len = decrypt(prepare_key_parity(key), cipher_text, buffer, cipher_len);
        buffer[len] = 0;
        // write file
        std::ofstream file;
        file.open("../result.csv", std::ios::app);
        if(file.is_open()) {
            file << (end-start)<<",";
        }

        printf("\nThe decrypted message was: %s \n\n", buffer);
        printf("\nThe key %ld matched %s \n%lf\n", key, buffer, (end-start));
    }

    MPI_Finalize();
    cleanup_cipher();
}
