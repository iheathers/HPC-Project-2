#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define DATA_SIZE 22


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

    if (rank == 0) {
        for (int dest = 0; dest < size; dest++) {
            // Send data to other processes
            MPI_Send(data + dest * data_per_process, data_per_process * sizeof(Fish), MPI_BYTE, dest, 0,
                     MPI_COMM_WORLD);
        }

        for (int i = 0; i < size; i++) {
            MPI_Recv(recv_buffer, data_per_process * sizeof(Fish), MPI_BYTE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }


        FILE *file = fopen("data_after.txt", "w");
        for (int i = 0; i < DATA_SIZE; i++) {
            fprintf(file, "x: %lf, y: %lf, distance: %lf, weight: %lf\n",
                    recv_buffer[i].x, recv_buffer[i].y, recv_buffer[i].distanceTraveled, recv_buffer[i].weight);
        }
        fclose(file);


    } else {
        // Receive data
        MPI_Recv(recv_buffer, data_per_process * sizeof(Fish), MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(recv_buffer, data_per_process * sizeof(Fish), MPI_BYTE, 0, 0, MPI_COMM_WORLD);

    }

    free(recv_buffer);
    free(data);

    MPI_Finalize();


}
