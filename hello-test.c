#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_DATA_ITEMS 13  // Define the total number of data items

int main(int argc, char *argv[]) {
    int rank, num_processes;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int data_to_send[NUM_DATA_ITEMS];

    if (rank == 0) {
        // Generate random numbers for data_to_send
        for (int i = 0; i < NUM_DATA_ITEMS; i++) {
            data_to_send[i] = (rand() % 10) + 1;
        }

        // Distribute data to other processes
        int send_count = NUM_DATA_ITEMS / num_processes;
        int extra_data = NUM_DATA_ITEMS % num_processes;

        int offset = 0;
        for (int i = 0; i < num_processes; i++) {
            int current_send_count = send_count + (i < extra_data ? 1 : 0);
            printf("Sending %d data items to process %d\n", current_send_count, i);
            MPI_Send(&data_to_send[offset], current_send_count, MPI_INT, i, 0, MPI_COMM_WORLD);
            offset += current_send_count;
        }
    }

    // Receive data
    int recv_count = NUM_DATA_ITEMS / num_processes + (rank < NUM_DATA_ITEMS % num_processes ? 1 : 0);
    int data_received[recv_count];
    MPI_Recv(data_received, recv_count, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Process received data
    for (int i = 0; i < recv_count; i++) {
        int squared = data_received[i] * data_received[i];
        printf("Process %d: Received %d from rank 0. Squared: %d\n", rank, data_received[i], squared);
    }


    MPI_Finalize();
    return 0;
}
