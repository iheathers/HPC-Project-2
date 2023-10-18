//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <mpi.h>
//
//#define DATA_SIZE 20
//
//int main(int argc, char **argv) {
//    MPI_Init(&argc, &argv);
//
//    int rank, size;
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    MPI_Comm_size(MPI_COMM_WORLD, &size);
//
//    int *data = NULL;
//    int data_per_process = DATA_SIZE / size;
//    int *recv_buffer = (int *) malloc(data_per_process * sizeof(int));
//
//    // Master process generates data
//    if (rank == 0) {
//        data = (int *) malloc(DATA_SIZE * sizeof(int));
//        for (int i = 0; i < DATA_SIZE; i++) {
//            data[i] = i;
//        }
//
//        // Output data to a file
//        FILE *file = fopen("data_before.txt", "w");
//        for (int i = 0; i < DATA_SIZE; i++) {
//            fprintf(file, "%d ", data[i]);
//        }
//        fclose(file);
//    }
//
//    // Distribute data to all processes
//    MPI_Scatter(data, data_per_process, MPI_INT, recv_buffer, data_per_process, MPI_INT, 0, MPI_COMM_WORLD);
//
//    // Workers send data back to the master
//    MPI_Gather(recv_buffer, data_per_process, MPI_INT, data, data_per_process, MPI_INT, 0, MPI_COMM_WORLD);
//
//    // Master process outputs the received data to a file
//    if (rank == 0) {
//        FILE *file = fopen("data_after.txt", "w");
//        for (int i = 0; i < DATA_SIZE; i++) {
//            fprintf(file, "%d ", data[i]);
//        }
//        fclose(file);
//
//        free(data);
//    }
//
//    free(recv_buffer);
//    MPI_Finalize();
//
//    return 0;
//}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define DATA_SIZE 20

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *data = NULL;
    int data_per_process = DATA_SIZE / size;
    int *recv_buffer = (int *) malloc(data_per_process * sizeof(int));

    // Master process generates data
    if (rank == 0) {
        data = (int *) malloc(DATA_SIZE * sizeof(int));
        for (int i = 0; i < DATA_SIZE; i++) {
            data[i] = i;
        }

        // Output data to a file
        FILE *file = fopen("data_before.txt", "w");
        for (int i = 0; i < DATA_SIZE; i++) {
            fprintf(file, "%d ", data[i]);
        }
        fclose(file);
    }

    // Distribute data to all processes
    if (rank == 0) {
        for (int dest = 0; dest < size; dest++) {
            if (dest != 0) {
                MPI_Send(data + dest * data_per_process, data_per_process, MPI_INT, dest, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Recv(recv_buffer, data_per_process, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Workers send data back to the master
    if (rank != 0) {
        MPI_Send(recv_buffer, data_per_process, MPI_INT, 0, 1, MPI_COMM_WORLD);
    } else {
        for (int source = 1; source < size; source++) {
            MPI_Recv(data + source * data_per_process, data_per_process, MPI_INT, source, 1, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
        }

        // Master process outputs the received data to a file
        FILE *file = fopen("data_after.txt", "w");
        for (int i = 0; i < DATA_SIZE; i++) {
            fprintf(file, "%d ", data[i]);
        }
        fclose(file);

        free(data);
    }

    free(recv_buffer);
    MPI_Finalize();

    return 0;
}
