#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_FISHES 10

// Define the Fish structure
typedef struct {
    double x, y;
    double distanceTraveled;
    double weight;
} Fish;

// Initialize the state of the lake with random fish positions
void initializeInitialLakeState(Fish *fishes, int numFishes) {
    for (int i = 0; i < numFishes; i++) {
        fishes[i].x = ((double) rand() / RAND_MAX) * 200.0 - 100.0;
        fishes[i].y = ((double) rand() / RAND_MAX) * 200.0 - 100.0;
        fishes[i].distanceTraveled = 0.0;
        fishes[i].weight = 1.0;
    }
}

// Write the fish data to a file
void writeFishDataToFile(const char *filename, Fish *fishes, int numFish) {
    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        for (int i = 0; i < numFish; i++) {
            fprintf(file, "%.2f %.2f %.2f %.2f\n", fishes[i].x, fishes[i].y,
                    fishes[i].distanceTraveled, fishes[i].weight);
        }
        fclose(file);
    } else {
        fprintf(stderr, "Error opening file for writing: %s\n", filename);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
}

int main(int argc, char *argv[]) {
    // Initialize MPI environment
    MPI_Init(&argc, &argv);

    int numtasks, taskid;
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

    Fish *fishes;
    int numFishInWorker, numFishToDistribute;
    int mtype;
    int offset = 0;

    // Master process
    if (taskid == 0) {
        srand(time(NULL));
        fishes = (Fish *) malloc(NUM_FISHES * sizeof(Fish));

        if (fishes == NULL) {
            fprintf(stderr, "Memory allocation failed. Exiting...\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // Initialize fish positions and save to file
        initializeInitialLakeState(fishes, NUM_FISHES);
        writeFishDataToFile("fish_data_initial.txt", fishes, NUM_FISHES);

        // Calculate how many fish data should be sent to each worker
        numFishToDistribute = NUM_FISHES / numtasks;
        int extra = NUM_FISHES % numtasks;

        // Distribute fish data to worker processes
        for (int dest = 0; dest < numtasks; dest++) {
            numFishInWorker = (dest < extra) ? numFishToDistribute + 1 : numFishToDistribute;

            mtype = 1; // Message type indicating it's FROM_MASTER
            MPI_Send(&numFishInWorker, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(fishes + offset, numFishInWorker * sizeof(Fish), MPI_BYTE, dest, mtype, MPI_COMM_WORLD);

            offset += numFishInWorker;
        }
    } else { // Worker processes
        mtype = 1; // Message type indicating it's FROM_MASTER
        MPI_Recv(&numFishInWorker, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        fishes = (Fish *) malloc(numFishInWorker * sizeof(Fish));
        MPI_Recv(fishes, numFishInWorker * sizeof(Fish), MPI_BYTE, 0, mtype, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Send received data back to the master
        mtype = 2; // Message type indicating it's FROM_WORKER
        MPI_Send(&numFishInWorker, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD);
        MPI_Send(fishes, numFishInWorker * sizeof(Fish), MPI_BYTE, 0, mtype, MPI_COMM_WORLD);
        free(fishes);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (taskid == 0) {
        // Master process collects data from workers
        Fish *masterFishes = (Fish *) malloc(NUM_FISHES * sizeof(Fish));
        offset = 0;

        for (int source = 1; source < numtasks; source++) {
            mtype = 2; // Message type indicating it's FROM_WORKER
            MPI_Recv(&numFishInWorker, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            fishes = (Fish *) malloc(numFishInWorker * sizeof(Fish));
            MPI_Recv(fishes, numFishInWorker * sizeof(Fish), MPI_BYTE, source, mtype, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);

            char recvFileName[50];
            snprintf(recvFileName, sizeof(recvFileName), "fish_data_received_%d.txt", source);
            writeFishDataToFile(recvFileName, fishes, numFishInWorker);

            // Merge received data into master data
            for (int i = 0; i < numFishInWorker; i++) {
                masterFishes[offset + i] = fishes[i];
            }

            free(fishes);
            offset += numFishInWorker;
        }

        // Save the final combined data
        writeFishDataToFile("fish_data_final.txt", masterFishes, NUM_FISHES);
        free(masterFishes);
    }

    MPI_Finalize();
    return 0;
}
