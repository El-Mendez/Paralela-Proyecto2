#include <openssl/evp.h>
#include <cstring>
#include <mpi.h>
#include "crypto.h"

#define KEY 2L
#define MAX_KEY (1L << 56)
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

int main(int argc, char** argv) {
    init_cipher();
    unsigned char cipher_text[BUFFER_SIZE];
    unsigned char buffer[BUFFER_SIZE];
    int cipher_len = get_encrypted_secret(cipher_text);

    // init mpi
    int size, rank;
    MPI_Init(nullptr, nullptr);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

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


    if (rank == 0) {
        MPI_Wait(&request, &status);
        int len = decrypt(prepare_key_parity(key), cipher_text, buffer, cipher_len);
        buffer[len] = 0;

        printf("\nThe key %ld (found by %d) matched %s", key, status.MPI_SOURCE, buffer);
    }

    MPI_Finalize();
    cleanup_cipher();
}
