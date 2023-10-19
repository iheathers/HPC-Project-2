# Part 1: MPI Communication

## Initial Implementation:

```c
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define DATA_SIZE 10

typedef struct {
    double x, y;
    double distanceTraveled;
    double weight;
} Fish;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    Fish *data = NULL;
    int data_per_process = DATA_SIZE / size;
    Fish *recv_buffer = (Fish *) malloc(data_per_process * sizeof(Fish));

    // Master process generates Fish data
    if (rank == 0) {
        data = (Fish *) malloc(DATA_SIZE * sizeof(Fish));
        for (int i = 0; i < DATA_SIZE; i++) {
            data[i].x = i * 0.5;
            data[i].y = i * 0.5;
            data[i].distanceTraveled = 0.0;
            data[i].weight = 1.0;
        }

        // Output data to a file
        FILE *file = fopen("data_before.txt", "w");
        for (int i = 0; i < DATA_SIZE; i++) {
            fprintf(file, "x: %lf, y: %lf, distance: %lf, weight: %lf\n",
                    data[i].x, data[i].y, data[i].distanceTraveled, data[i].weight);
        }
        fclose(file);
    }

    // Distribute Fish data to all processes
    if (rank == 0) {
        for (int dest = 0; dest < size; dest++) {
            // Send data to other processes as well as master
            MPI_Send(data + dest * data_per_process, data_per_process * sizeof(Fish), MPI_BYTE, dest, 0,
                     MPI_COMM_WORLD);
        }
    } else {
        // Receive data
        MPI_Recv(recv_buffer, data_per_process * sizeof(Fish), MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Add a barrier to ensure that all processes have received their data
    MPI_Barrier(MPI_COMM_WORLD);

    // Double the received data before sending it back for debugging purpose
    if (rank != 0) {
    
//        for (int i = 0; i < data_per_process; i++) {
//            recv_buffer[i].x *= 2.0;
//            recv_buffer[i].y *= 2.0;
//        }

        // Send the modified data back to the master
        MPI_Send(recv_buffer, data_per_process * sizeof(Fish), MPI_BYTE, 0, 1, MPI_COMM_WORLD);
    } else {
        // Master receives data from workers
        for (int source = 1; source < size; source++) {
            MPI_Recv(data + source * data_per_process, data_per_process * sizeof(Fish), MPI_BYTE, source, 1,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Master process outputs the received Fish data to a file
        FILE *file = fopen("data_after.txt", "w");
        for (int i = 0; i < DATA_SIZE; i++) {
            fprintf(file, "x: %lf, y: %lf, distance: %lf, weight: %lf\n",
                    data[i].x, data[i].y, data[i].distanceTraveled, data[i].weight);
        }
        fclose(file);

        free(data);
    }

    free(recv_buffer);
    MPI_Finalize();

    return 0;
}

```

### Overview

The provided code implements an MPI program where a master process (`rank = 0`) initializes an array of data
representing fishes and distributes this data among all available processes. After the distribution, each non-master
process modifies its piece of data (this modification is commented out in the provided code. Modification was done only
for debugging purpose) and sends it back to the
master process, which then writes the modified data to an output file.

The distribution of data to the processes is done based on the formula: `data_per_process = DATA_SIZE / size`,
where `DATA_SIZE` is the total number of data points and `size` is the total number of processes. However, this approach
does not evenly distribute the data among processes if `DATA_SIZE` is not a multiple of `size`.

### Data Distribution

Based on the output with `DATA_SIZE = 10` and `4 processes`, the distribution of data is as follows:

| **Data Before**     (data_before.txt)                                       | **Data After**                      (data_after.txt)                              |
|-----------------------------------------------------------------------------|-----------------------------------------------------------------------------------|
| x: 0.000000, y: 0.000000, distance: 0.000000, weight: 1.000000 -> Process 0 | x: 0.000000, y: 0.000000, distance: 0.000000, weight: 1.000000 -> Original        |
| x: 0.500000, y: 0.500000, distance: 0.000000, weight: 1.000000 -> Process 0 | x: 0.500000, y: 0.500000, distance: 0.000000, weight: 1.000000 -> Original        |
| x: 1.000000, y: 1.000000, distance: 0.000000, weight: 1.000000 -> Process 1 | x: 1.000000, y: 1.000000, distance: 0.000000, weight: 1.000000 -> Received from 1 |
| x: 1.500000, y: 1.500000, distance: 0.000000, weight: 1.000000 -> Process 1 | x: 1.500000, y: 1.500000, distance: 0.000000, weight: 1.000000 -> Received from 1 |
| x: 2.000000, y: 2.000000, distance: 0.000000, weight: 1.000000 -> Process 2 | x: 2.000000, y: 2.000000, distance: 0.000000, weight: 1.000000 -> Received from 2 |
| x: 2.500000, y: 2.500000, distance: 0.000000, weight: 1.000000 -> Process 2 | x: 2.500000, y: 2.500000, distance: 0.000000, weight: 1.000000 -> Received from 2 |
| x: 3.000000, y: 3.000000, distance: 0.000000, weight: 1.000000 -> Process 3 | x: 3.000000, y: 3.000000, distance: 0.000000, weight: 1.000000 -> Received from 3 |
| x: 3.500000, y: 3.500000, distance: 0.000000, weight: 1.000000 -> Process 3 | x: 3.500000, y: 3.500000, distance: 0.000000, weight: 1.000000 -> Received from 3 |
| x: 4.000000, y: 4.000000, distance: 0.000000, weight: 1.000000 -> Process 0 | x: 4.000000, y: 4.000000, distance: 0.000000, weight: 1.000000 -> Original        |
| x: 4.500000, y: 4.500000, distance: 0.000000, weight: 1.000000 -> Process 0 | x: 4.500000, y: 4.500000, distance: 0.000000, weight: 1.000000 -> Original        |

- `DATA_SIZE` is 10.
- There are 4 processes (`size = 4`).
- `data_per_process` is thus 2.5, but since this is a double value and indices must be integers, when used in
  calculations, it implicitly gets converted to 2. Therefore, each process (apart from the first one where all the
  remaining data will be given) will handle 2
  fishes.

From the provided output:

- Process 0 handles the fishes with `x: 0.0, y: 0.0` and `x: 0.5, y: 0.5`, and the fishes with `x: 4.0, y: 4.0`
  and `x: 4.5, y: 4.5`.
- Process 1 handles the fishes with `x: 1.0, y: 1.0` and `x: 1.5, y: 1.5`.
- Process 2 handles the fishes with `x: 2.0, y: 2.0` and `x: 2.5, y: 2.5`.
- Process 3 handles the fishes with `x: 3.0, y: 3.0` and `x: 3.5, y: 3.5`.

Clearly, the distribution is uneven. Process 0 handles 4 fishes, while the rest of the processes handle only 2 fishes
each.

### Issue

The issue arises due to the way data is sent to processes in the loop:

```c
for (int dest = 0; dest < size; dest++) {
    MPI_Send(data + dest * data_per_process, ...);
}
```

Since `data_per_process` is implicitly 2 for all `dest`, it leads to Process 0 handling the first 2 fishes and the last
2 fishes.

So, we had to think of a way to distribute the data more evenly.

## Second Implementation

### Solution Using Offset

To solve the problem, we used an offset approach similar to the example on the lecture. We added an offset
to `data_per_process` based on the
process's rank. The idea is to distribute the remaining data points (after dividing `DATA_SIZE` by `size`) among the
first few processes.

Pseudocode for this solution is as follows:

```c
int remaining_data = DATA_SIZE % size;
int offset = (rank < remaining_data) ? rank : remaining_data;

MPI_Send(data + dest * data_per_process + offset, ...);
```

Building on this idea, we implemented the following solution:

```c
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

```

For testing and debuggin purpose, we create different files for the initial data, the data received by each worker, and
the final
data.

### Overview

n the given program, there is a master process (with rank 0) that first generates some random data representing fishes
in a lake. This master process then intends to distribute these fishes among different worker processes. Post
distribution, the workers send the data back to the master. Finally, the master gathers all data received from workers
and writes it to the final file.

### Data Distribution

Based on the output with `NUM_FISHES = 10` and `4 processes`, the distribution of data is as follows:

In `fish_data_received_1.txt`:

| **fish_data_received_1.txt** (Process 1) |
|------------------------------------------|
| 62.74 54.32 0.00 1.00                    |
| -77.59 18.70 0.00 1.00                   |
| 35.35 -85.62 0.00 1.00                   |

In `fish_data_received_2.txt`:

| **fish_data_received_2.txt** (Process 2) |
|------------------------------------------|
| -45.09 52.33 0.00 1.00                   |
| -36.99 -63.77 0.00 1.00                  |

In `fish_data_received_3.txt`:

| **fish_data_received_3.txt** (Process 3) |
|------------------------------------------|
| 54.87 5.47 0.00 1.00                     |
| -17.11 2.62 0.00 1.00                    |

The above output hows the fish data received by each of the respective worker processes after distribution by the
master.

### Output

Initial data (`fish_data_initial.txt`): Represents the state of the lake before distribution.
Final data sent back to process 0 (`fish_data_final.txt`): Shows the consolidated fish data received by the master from
all workers.

| Data Before   (fish_data_initial.txt)         | Data Final  (fish_data_final.txt)                    |
|-----------------------------------------------|------------------------------------------------------|
| 39.96 20.01 0.00 1.00    -> Sent to Process 0 | 62.74 54.32 0.00 1.00     -> Received from Process 1 |
| -34.89 -3.86 0.00 1.00   -> Sent to Process 0 | -77.59 18.70 0.00 1.00    -> Received from Process 1 |
| -94.34 -4.60 0.00 1.00   -> Sent to Process 0 | 35.35 -85.62 0.00 1.00    -> Received from Process 1 |
| 62.74 54.32 0.00 1.00    -> Sent to Process 1 | -45.09 52.33 0.00 1.00    -> Received from Process 2 |
| -77.59 18.70 0.00 1.00   -> Sent to Process 1 | -36.99 -63.77 0.00 1.00   -> Received from Process 2 |
| 35.35 -85.62 0.00 1.00   -> Sent to Process 1 | 54.87 5.47 0.00 1.00      -> Received from Process 3 |
| -45.09 52.33 0.00 1.00   -> Sent to Process 2 | -17.11 2.62 0.00 1.00     -> Received from Process 3 |
| -36.99 -63.77 0.00 1.00  -> Sent to Process 2 | 0.00 0.00 0.00 0.00                                  |
| 54.87 5.47 0.00 1.00     -> Sent to Process 3 | 0.00 0.00 0.00 0.00                                  |
| -17.11 2.62 0.00 1.00    -> Sent to Process 3 | 0.00 0.00 0.00 0.00                                  |

With this approach:

- Process 0 is supposed to handle 3 fishes (2 fishes from the initial data and 1 fish from the remaining data):
- Process 1 is supposed to handle 3 fishes (2 fishes from the initial data and 1 fish from the remaining data):
- Process 2 is supposed to handle 2 fishes (2 fishes from the initial data and 0 fish from the remaining data):
- Process 3 is supposed to handle 2 fishes (2 fishes from the initial data and 0 fish from the remaining data):

This gives a more even distribution of the data.

### Issue

However, the final data in `fish_data_final.txt` has 7 valid fish entities, but the last 3 entries are zeroes. This
indicates that the data for the last 3 fishes was not collected properly, leading to discrepancies in the final results.

### Reason for the Discrepancy:**

The issue arises from how the master process (process 0) distributes and collects data. Specifically:

- When the master is distributing data, it also sends data to itself (as `dest` goes from 0 to `numtasks-1`). Given
  that `MPI_Send` is a blocking call, the master waits for a corresponding `MPI_Recv` to pick up this data.

- However, while the master is blocked on the `MPI_Send`, there isn't an immediate `MPI_Recv` to collect the data it
  sent to itself, causing a deadlock. So, to avoid that, we never received data sent by master to itself.

### Solution using `MPI_Isend`:**

To avoid the aforementioned deadlock situation, we finally figured out a way to use non-blocking sends, such
as `MPI_Isend`. Here's how we did it:

1. Use `MPI_Isend` instead of `MPI_Send` when distributing fish data.
2. Use `MPI_Request` to manage the non-blocking send.
3. Add a corresponding `MPI_Wait` after the sends to ensure that they complete.

Here's a code snippet for the master process distribution using `MPI_Isend`:

```c
MPI_Request request;

// ... previous code ...

// Distribute fish data to worker processes
for (int dest = 0; dest < numtasks; dest++) {
    numFishInWorker = (dest < extra) ? numFishToDistribute + 1 : numFishToDistribute;

    mtype = 1; // Message type indicating it's FROM_MASTER

    // Non-blocking send
    MPI_Isend(&numFishInWorker, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD, &request);
    MPI_Isend(fishes + offset, numFishInWorker * sizeof(Fish), MPI_BYTE, dest, mtype, MPI_COMM_WORLD, &request);
    
    offset += numFishInWorker;
}

// ... remaining code ...
```

## Final Implementation

```c
#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_FISHES 22

typedef struct {
    double x, y;
    double distanceTraveled;
    double weight;
} Fish;

void initializeInitialLakeState(Fish *fishes, int numFishes) {
    for (int i = 0; i < numFishes; i++) {
        fishes[i].x = ((double) rand() / RAND_MAX) * 200.0 - 100.0;
        fishes[i].y = ((double) rand() / RAND_MAX) * 200.0 - 100.0;
        fishes[i].distanceTraveled = 0.0;
        fishes[i].weight = 1.0;
    }
}

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
    MPI_Init(&argc, &argv);

    int numtasks, taskid;
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

    Fish *fishes;
    int numFishInWorker, numFishToDistribute;
    int mtype;
    int offset = 0;

    // Master generates and distributes fish data
    if (taskid == 0) {
        srand(time(NULL));
        fishes = (Fish *) malloc(NUM_FISHES * sizeof(Fish));

        if (fishes == NULL) {
            fprintf(stderr, "Memory allocation failed. Exiting...\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // Initialize the fish positions and write to an initial file
        initializeInitialLakeState(fishes, NUM_FISHES);
        writeFishDataToFile("fish_data_initial.txt", fishes, NUM_FISHES);

        // Determine how many fish to distribute to each worker
        numFishToDistribute = NUM_FISHES / numtasks;
        int extra = NUM_FISHES % numtasks;

        // Distribute fish data using non-blocking sends
        for (int dest = 0; dest < numtasks; dest++) {
            numFishInWorker = (dest < extra) ? numFishToDistribute + 1 : numFishToDistribute;
            mtype = 1; //  // Tag for sending from master

            // Use non-blocking send
            MPI_Request request;
            MPI_Isend(&numFishInWorker, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD, &request);
            MPI_Isend(fishes + offset, numFishInWorker * sizeof(Fish), MPI_BYTE, dest, mtype, MPI_COMM_WORLD, &request);

            offset += numFishInWorker;
        }
    }

// All processes (master + workers) receive fish data
    mtype = 1; // // Tag for receiving from master
    MPI_Recv(&numFishInWorker, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    fishes = (Fish *) malloc(numFishInWorker * sizeof(Fish));
    MPI_Recv(fishes, numFishInWorker * sizeof(Fish), MPI_BYTE, 0, mtype, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Send the received data back to the master
    mtype = 2; // // Tag for sending to master
    MPI_Send(&numFishInWorker, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD);
    MPI_Send(fishes, numFishInWorker * sizeof(Fish), MPI_BYTE, 0, mtype, MPI_COMM_WORLD);
    free(fishes);

    MPI_Barrier(MPI_COMM_WORLD); // Synchronize all processes

    // The master node receives data from all workers, including itself
    if (taskid == 0) {
        Fish *masterFishes = (Fish *) malloc(NUM_FISHES * sizeof(Fish));
        offset = 0;

        for (int source = 0; source < numtasks; source++) {
            mtype = 2; // // Tag for receiving from worker
            MPI_Recv(&numFishInWorker, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            fishes = (Fish *) malloc(numFishInWorker * sizeof(Fish));
            MPI_Recv(fishes, numFishInWorker * sizeof(Fish), MPI_BYTE, source, mtype, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);


            // Write received data to separate files for each worker
            char recvFileName[50];
            snprintf(recvFileName, sizeof(recvFileName), "fish_data_received_%d.txt", source);
            writeFishDataToFile(recvFileName, fishes, numFishInWorker);


            // Merge all received data into the master array
            for (int i = 0; i < numFishInWorker; i++) {
                masterFishes[offset + i] = fishes[i];
            }

            free(fishes);
            offset += numFishInWorker;
        }

        // Write the final merged data to a master file
        writeFishDataToFile("fish_data_final.txt", masterFishes, NUM_FISHES);
        free(masterFishes);
    }

    MPI_Finalize();
    return 0;
}

```
