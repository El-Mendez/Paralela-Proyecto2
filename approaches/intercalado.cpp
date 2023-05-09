#include "../utils/utils.h"
#include "approach.h"
#include <mpi.h>

long crack_password(int rank, int size, unsigned char* cipher_text, int cipher_len, unsigned char* buffer) {
    MPI_Request request;
    MPI_Status status;
    long key = 0L;
    int found = 0;

    // fuerza bruta
    MPI_Irecv(&key, 1, MPI_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
    for (long i = rank; i < MAX_KEY; i += size) {
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

    MPI_Wait(&request, &status);
    return key;
}
