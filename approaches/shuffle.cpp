#include "../utils/utils.h"
#include "approach.h"
#include <mpi.h>
#include <memory>

approach_result crack_password(int rank, int size, unsigned char* cipher_text, int cipher_len, unsigned char* buffer) {
    // dividir el trabajo entre nodos
    long work_per_node = MAX_KEY / size;
    long lower = work_per_node * rank;
    long upper = work_per_node * (rank + 1) - 1;
    if (rank == size-1) {
        upper = MAX_KEY;
    }
    srand((unsigned) time(nullptr) + rank);

    MPI_Request request;
    MPI_Status status;
    approach_result result {
            -1, -1
    };
    long key = 0L;
    int found = 0;
    long range = upper - lower;
    auto was_checked = std::make_unique<bool[]>(range);

    // fuerza bruta shuffle
    MPI_Irecv(&key, 1, MPI_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
    for (long i = 0; i < range; i++) {
        MPI_Test(&request, &found, &status);
        if (found) {
            break;
        }
        long random_key = lower + (rand() % range);
        while(was_checked[random_key - lower]) {
            random_key = lower + (rand() % range);
        }
        was_checked[random_key - lower] = true;

        if (key_matches(random_key, cipher_text, cipher_len, buffer)) {
            key = random_key;
            result.key = key;
            result.rank = rank;

            for (int node = 0; node < size; node++)
                MPI_Send(&key, 1, MPI_LONG, node, 0, MPI_COMM_WORLD);
            break;
        }
    }

    if (result.rank != rank) {
        if (!found) {
            MPI_Wait(&request, &status);
        }
        result.key = key;
        result.rank = status.MPI_SOURCE;
    }
    return result;
}
