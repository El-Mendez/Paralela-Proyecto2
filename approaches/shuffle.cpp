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

    //printf("Process %d: %ld - %ld \n", rank, lower, upper);

    MPI_Request request;
    MPI_Status status;
    approach_result result;
    long key = 0L;
    int found = 0;
    long range = upper - lower;
    auto was_checked = std::make_unique<bool[]>(range);

    // printf("Process %d: %ld - %ld \n", rank, lower, upper);

    // fuerza bruta shuffle
    MPI_Irecv(&key, 1, MPI_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
    for (long i = 0; i < range; i++) {
        MPI_Test(&request, &found, MPI_STATUS_IGNORE);
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

    MPI_Wait(&request, &status);
    return result;
}
