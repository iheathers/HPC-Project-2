////
//// Created by Pritam Suwal Shrestha on 17/10/2023.
////
//
//
//#include <mpi.h>
//#include <stdio.h>
//#include <stdlib.h>
//
//
//int main(int argc, char *argv[]) {
//    int rank, num_processes;
//
//    MPI_Init(&argc, &argv);
//
//    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//
//
////    int i = 0;
////    while (!i)
////        sleep(2);
//
//
//    if (rank == 0) {
//
//        for (int i = 0; i < num_processes; i++) {
//            // generate random number
////            generate random number between 1 to 10
//            int random_number = (rand() % 10) + 1;
//
//            printf("Sending %d to %d\n", random_number, i);
//
//            MPI_Send(&random_number, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
//        }
//
//
//        for (int i = 0; i < num_processes; i++) {
//            int recv_sq;
//            MPI_Recv(&recv_sq, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//            printf("Received %d from %d\n", recv_sq, i);
//        }
//
//    } else {
//
//        int recv;
//        MPI_Recv(&recv, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
////        printf("Received %d from 0\n", recv);
//
//
//        int squared = recv * recv;
//
//        MPI_Send(&squared, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
//    }
//
//
//    MPI_Finalize();
//}

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_DATA_ITEMS 5

int main(int argc, char *argv[]) {
    int rank, num_processes;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        // Create an array to store the random numbers
        int random_numbers[NUM_DATA_ITEMS];

        // Seed the random number generator
        srand(time(NULL));

        // Generate and send random numbers to other processes
        for (int i = 0; i < NUM_DATA_ITEMS; i++) {
            int random_number = (rand() % 10) + 1;
            random_numbers[i] = random_number;

            for (int dest = 1; dest < num_processes; dest++) {
                MPI_Send(&random_number, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
            }
        }

        // Receive and print squared values from other processes
        for (int i = 0; i < NUM_DATA_ITEMS; i++) {
            int squared_values[num_processes - 1];
            for (int src = 1; src < num_processes; src++) {
                int recv_sq;
                MPI_Recv(&recv_sq, 1, MPI_INT, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                squared_values[src - 1] = recv_sq;
            }
            printf("Random Number %d: %d\n", i, random_numbers[i]);
            for (int src = 1; src < num_processes; src++) {
                printf("Received squared value from Process %d: %d\n", src, squared_values[src - 1]);
            }
        }
    } else {
        // Receive random numbers and send squared values to the master process
        int recv;
        for (int i = 0; i < NUM_DATA_ITEMS; i++) {
            MPI_Recv(&recv, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            int squared = recv * recv;
            MPI_Send(&squared, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
}
