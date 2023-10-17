//
// Created by Pritam Suwal Shrestha on 17/10/2023.
//

#include <stdio.h>
#include <mpi.h>
#include <time.h>

void round_robin_basic(int rank, int num_processes){
    long int rand_mine, rand_prev;
    int rank_next = (rank + 1) % num_processes;

    int rank_prev = rank == 0 ? num_processes - 1 : rank - 1;

    MPI_Status status;

    srandom(time(NULL) + rank);

    printf("%d: Generating random number %ld\n", rank, rand_mine);

    if (rank % 2 == 0){
        MPI_Send(&rand_mine, 1, MPI_LONG, rank_next, 0, MPI_COMM_WORLD);
        printf("%d: Sending random number %ld to %d\n", rank, rand_mine, rank_next);

        MPI_Recv(&rand_prev, 1, MPI_LONG, rank_prev, 0, MPI_COMM_WORLD, &status);
        printf("%d: Receiving random %ld number from %d\n", rank, rand_prev, rank_prev);
    } else {

        MPI_Recv(&rand_prev, 1, MPI_LONG, rank_prev, 0, MPI_COMM_WORLD, &status);
        printf("%d: Receiving random number  %ld from %d\n", rank, rand_prev, rank_prev);

        MPI_Send(&rand_mine, 1, MPI_LONG, rank_next, 0, MPI_COMM_WORLD);
        printf("%d: Sending random number %ld to %d\n", rank, rand_mine, rank_next);
    }

    printf("%d: I had %ld,  %d had %ld\n", rank, rand_mine, rank_prev, rand_prev);

}

int main(int argc, char *argv[]){
    int rank, num_processes;

    MPI_Init(&argc, &argv);

    int i = 0;
    while (!i){
        sleep(2);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("%d: Hello (p=%d)\n", rank, num_processes);
    round_robin_basic(rank, num_processes);
    printf("%d: Goodbye (p=%d)\n", rank, num_processes);

    MPI_Finalize();
}
