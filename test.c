#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Define a constant for the number of fish
#define NUM_FISH 5

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

    Fish myFish; // Create a Fish structure

    if (rank == 0) {
        Fish *fishArray = (Fish *) malloc(NUM_FISH * sizeof(Fish));

        for (int i = 0; i < NUM_FISH; i++) {
            // Initialize the Fish structure with some values
            myFish.x = i;
            myFish.y = i + 1;
            myFish.distanceTraveled = 0.0;
            myFish.weight = i * 2.0;

            int dest_rank = i % num_processes; // Determine the destination rank

            // Send the Fish structure
            printf("Sending Fish to process %d: x = %lf, y = %lf, distanceTraveled = %lf, weight = %lf\n",
                   dest_rank, myFish.x, myFish.y, myFish.distanceTraveled, myFish.weight);
            MPI_Send(&myFish, sizeof(Fish), MPI_BYTE, dest_rank, 0, MPI_COMM_WORLD);
        }

        // Receive messages from other processes
        for (int i = 0; i < NUM_FISH; i++) {
            Fish receivedFish;
            int source_rank = i % num_processes; // Determine the source rank
            MPI_Recv(&receivedFish, sizeof(Fish), MPI_BYTE, source_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Received Fish from process %d: x = %lf, y = %lf, distanceTraveled = %lf, weight = %lf\n",
                   source_rank, receivedFish.x, receivedFish.y, receivedFish.distanceTraveled, receivedFish.weight);
            // Store a copy in the array
            fishArray[i] = receivedFish;
        }

        free(fishArray);
    } else {
        Fish receivedFish;
        MPI_Recv(&receivedFish, sizeof(Fish), MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Modify the received fish components
        receivedFish.x *= 2;
        receivedFish.y *= 2;
        receivedFish.distanceTraveled *= 2;
        receivedFish.weight *= 2;

        // Send the modified Fish structure back to process 0
        MPI_Send(&receivedFish, sizeof(Fish), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
