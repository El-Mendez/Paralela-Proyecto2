#include <mpi.h>
#include "crypto/crypto.h"
#include "utils/utils.h"

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


    if(rank == 0){
        printf("Ingrese el nombre del archivo con el mensaje a encriptar\n");
        char filename[BUFFER_SIZE];
        scanf("%s", filename);
        cipher_len = get_encrypted_secret(filename,cipher_text);
        printf("Encrypted message: %s\n", cipher_text);
        MPI_Bcast(&cipher_len, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&cipher_text, cipher_len, MPI_CHAR, 0, MPI_COMM_WORLD);
    }else{
        MPI_Bcast(&cipher_len, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&cipher_text, cipher_len, MPI_CHAR, 0, MPI_COMM_WORLD);
    }


    // dividir el trabajo entre nodos
    long work_per_node = MAX_KEY / size;
    long lower = work_per_node * rank;
    long upper = work_per_node * (rank + 1) - 1;
    if (rank == size-1) {
        upper = MAX_KEY;
    }

    printf("Process %d: %ld - %ld \n", rank, lower, upper);

    MPI_Request request;
    MPI_Status status;
    long key = 0L;
    int found = 0;

    // fuerza bruta
    MPI_Irecv(&key, 1, MPI_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
    for (long i = lower; i < upper; i++) {
        MPI_Test(&request, &found, MPI_STATUS_IGNORE);
        if (found) {
            break;
        }

        if (key_matches(i, cipher_text, cipher_len, buffer)) {
            key = i;
            for (int node = 0; node < size; node++)
                MPI_Send(&key, 1, MPI_LONG, node, 0, MPI_COMM_WORLD);
            break;
        }
    }


    // imprimir resultados
    if (rank == 0) {
        MPI_Wait(&request, &status);
        int len = decrypt(prepare_key_parity(key), cipher_text, buffer, cipher_len);
        buffer[len] = 0;

        printf("\nThe key %ld (found by %d) matched %s", key, status.MPI_SOURCE, buffer);
    }

    MPI_Finalize();
    cleanup_cipher();
}
