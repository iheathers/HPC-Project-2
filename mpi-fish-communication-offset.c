//
// Created by Pritam Suwal Shrestha on 18/10/2023.
//


#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


#define NUM_FISHES 10

typedef struct {
    double x, y;
    double distanceTraveled;
    double weight;
} Fish;

int main(int argc, char *argv[]) {
    int rank, num_processes;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    MPI_Finalize();

}
