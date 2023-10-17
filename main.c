#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);

    int i = 0;
    while (!i)
        sleep(2);

    // Get the rank of the current process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get the total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("Hello, World! This is process %d out of %d processes.\n", rank, size);

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
