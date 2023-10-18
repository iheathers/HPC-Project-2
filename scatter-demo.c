//////#include <stdio.h>
//////#include <stdlib.h>
//////#include <string.h>
//////#include <mpi.h>
//////
//////#define DATA_SIZE 22
//////
//////typedef struct {
//////    double x, y;
//////    double distanceTraveled;
//////    double weight;
//////} Fish;
//////
//////int main(int argc, char **argv) {
//////    MPI_Init(&argc, &argv);
//////
//////    int rank, size;
//////    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//////    MPI_Comm_size(MPI_COMM_WORLD, &size);
//////
//////    Fish *data = NULL;
//////    int data_per_process = DATA_SIZE / size;
//////    Fish *recv_buffer = (Fish *) malloc(data_per_process * sizeof(Fish));
//////
//////    // Master process generates Fish data
//////    if (rank == 0) {
//////        data = (Fish *) malloc(DATA_SIZE * sizeof(Fish));
//////        for (int i = 0; i < DATA_SIZE; i++) {
//////            data[i].x = i * 0.5;
//////            data[i].y = i * 0.5;
//////            data[i].distanceTraveled = 0.0;
//////            data[i].weight = 1.0;
//////        }
//////
//////        // Output data to a file
//////        FILE *file = fopen("data_before.txt", "w");
//////        for (int i = 0; i < DATA_SIZE; i++) {
//////            fprintf(file, "x: %lf, y: %lf, distance: %lf, weight: %lf\n",
//////                    data[i].x, data[i].y, data[i].distanceTraveled, data[i].weight);
//////        }
//////        fclose(file);
//////    }
//////
//////    // Distribute Fish data to all processes
//////    if (rank == 0) {
//////        for (int dest = 0; dest < size; dest++) {
//////            if (dest != 0) {
//////                MPI_Send(data + dest * data_per_process, data_per_process * sizeof(Fish), MPI_BYTE, dest, 0,
//////                         MPI_COMM_WORLD);
//////            }
//////        }
//////    } else {
//////        MPI_Recv(recv_buffer, data_per_process * sizeof(Fish), MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//////    }
//////
//////    // Workers send Fish data back to the master
//////    if (rank != 0) {
//////        MPI_Send(recv_buffer, data_per_process * sizeof(Fish), MPI_BYTE, 0, 1, MPI_COMM_WORLD);
//////    } else {
//////        for (int source = 1; source < size; source++) {
//////            MPI_Recv(data + source * data_per_process, data_per_process * sizeof(Fish), MPI_BYTE, source, 1,
//////                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//////        }
//////
//////        // Master process outputs the received Fish data to a file
//////        FILE *file = fopen("data_after.txt", "w");
//////        for (int i = 0; i < DATA_SIZE; i++) {
//////            fprintf(file, "x: %lf, y: %lf, distance: %lf, weight: %lf\n",
//////                    data[i].x, data[i].y, data[i].distanceTraveled, data[i].weight);
//////        }
//////        fclose(file);
//////
//////        free(data);
//////    }
//////
//////    free(recv_buffer);
//////    MPI_Finalize();
//////
//////    return 0;
//////}
////
////
////#include <stdio.h>
////#include <stdlib.h>
////#include <string.h>
////#include <mpi.h>
////
////#define DATA_SIZE 22
////
////typedef struct {
////    double x, y;
////    double distanceTraveled;
////    double weight;
////} Fish;
////
////int main(int argc, char **argv) {
////    MPI_Init(&argc, &argv);
////
////    int rank, size;
////    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
////    MPI_Comm_size(MPI_COMM_WORLD, &size);
////
////    Fish *data = NULL;
////    int data_per_process = DATA_SIZE / size;
////    Fish *recv_buffer = (Fish *) malloc(data_per_process * sizeof(Fish));
////
////    // Master process generates Fish data
////    if (rank == 0) {
////        data = (Fish *) malloc(DATA_SIZE * sizeof(Fish));
////        for (int i = 0; i < DATA_SIZE; i++) {
////            data[i].x = i * 0.5;
////            data[i].y = i * 0.5;
////            data[i].distanceTraveled = 0.0;
////            data[i].weight = 1.0;
////        }
////
////        // Output data to a file
////        FILE *file = fopen("data_before.txt", "w");
////        for (int i = 0; i < DATA_SIZE; i++) {
////            fprintf(file, "x: %lf, y: %lf, distance: %lf, weight: %lf\n",
////                    data[i].x, data[i].y, data[i].distanceTraveled, data[i].weight);
////        }
////        fclose(file);
////    }
////
////    // Distribute Fish data to all processes
////    if (rank == 0) {
////        for (int dest = 0; dest < size; dest++) {
////            if (dest != 0) {
////                // Double the x and y fields before sending
////                for (int i = dest * data_per_process; i < (dest + 1) * data_per_process; i++) {
////                    data[i].x *= 2.0;
////                    data[i].y *= 2.0;
////                    // You can similarly double other fields like distanceTraveled and weight if needed.
////                }
////                MPI_Send(data + dest * data_per_process, data_per_process * sizeof(Fish), MPI_BYTE, dest, 0,
////                         MPI_COMM_WORLD);
////            }
////        }
////    } else {
////        MPI_Recv(recv_buffer, data_per_process * sizeof(Fish), MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
////    }
////
////    // Workers send Fish data back to the master
////    if (rank != 0) {
////        MPI_Send(recv_buffer, data_per_process * sizeof(Fish), MPI_BYTE, 0, 1, MPI_COMM_WORLD);
////    } else {
////        for (int source = 1; source < size; source++) {
////            MPI_Recv(data + source * data_per_process, data_per_process * sizeof(Fish), MPI_BYTE, source, 1,
////                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
////        }
////
////        // Master process outputs the received Fish data to a file
////        FILE *file = fopen("data_after.txt", "w");
////        for (int i = 0; i < DATA_SIZE; i++) {
////            fprintf(file, "x: %lf, y: %lf, distance: %lf, weight: %lf\n",
////                    data[i].x, data[i].y, data[i].distanceTraveled, data[i].weight);
////        }
////        fclose(file);
////
////        free(data);
////    }
////
////    free(recv_buffer);
////    MPI_Finalize();
////
////    return 0;
////}
//
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <mpi.h>
//
//#define DATA_SIZE 22
//
//typedef struct {
//    double x, y;
//    double distanceTraveled;
//    double weight;
//} Fish;
//
//int main(int argc, char **argv) {
//    MPI_Init(&argc, &argv);
//
//    int rank, size;
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    MPI_Comm_size(MPI_COMM_WORLD, &size);
//
//    Fish *data = NULL;
//    int data_per_process = DATA_SIZE / size;
//    Fish *recv_buffer = (Fish *) malloc(data_per_process * sizeof(Fish));
//
//    // Master process generates Fish data
//    if (rank == 0) {
//        data = (Fish *) malloc(DATA_SIZE * sizeof(Fish));
//        for (int i = 0; i < DATA_SIZE; i++) {
//            data[i].x = i * 0.5;
//            data[i].y = i * 0.5;
//            data[i].distanceTraveled = 0.0;
//            data[i].weight = 1.0;
//        }
//
//        // Output data to a file
//        FILE *file = fopen("data_before.txt", "w");
//        for (int i = 0; i < DATA_SIZE; i++) {
//            fprintf(file, "x: %lf, y: %lf, distance: %lf, weight: %lf\n",
//                    data[i].x, data[i].y, data[i].distanceTraveled, data[i].weight);
//        }
//        fclose(file);
//    }
//
//    // Distribute Fish data to all processes
//    if (rank == 0) {
//        for (int dest = 0; dest < size; dest++) {
//            if (dest != 0) {
//                // Double the x and y fields before sending
//                for (int i = dest * data_per_process; i < (dest + 1) * data_per_process; i++) {
//                    data[i].x *= 2.0;
//                    data[i].y *= 2.0;
//                    // You can similarly double other fields like distanceTraveled and weight if needed.
//                }
//                MPI_Send(data + dest * data_per_process, data_per_process * sizeof(Fish), MPI_BYTE, dest, 0,
//                         MPI_COMM_WORLD);
//            }
//        }
//    } else {
//        MPI_Recv(recv_buffer, data_per_process * sizeof(Fish), MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//
//        // Double the received data before sending it back
//        for (int i = 0; i < data_per_process; i++) {
//            recv_buffer[i].x *= 2.0;
//            recv_buffer[i].y *= 2.0;
//            // You can similarly double other fields like distanceTraveled and weight if needed.
//        }
//        MPI_Send(recv_buffer, data_per_process * sizeof(Fish), MPI_BYTE, 0, 1, MPI_COMM_WORLD);
//    }
//
//    // Workers send Fish data back to the master
//    if (rank != 0) {
//        // No need to double the data here; it's already doubled by worker processes
//        MPI_Send(recv_buffer, data_per_process * sizeof(Fish), MPI_BYTE, 0, 1, MPI_COMM_WORLD);
//    } else {
//        for (int source = 1; source < size; source++) {
//            MPI_Recv(data + source * data_per_process, data_per_process * sizeof(Fish), MPI_BYTE, source, 1,
//                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//        }
//
//        // Master process outputs the received Fish data to a file
//        FILE *file = fopen("data_after.txt", "w");
//        for (int i = 0; i < DATA_SIZE; i++) {
//            fprintf(file, "x: %lf, y: %lf, distance: %lf, weight: %lf\n",
//                    data[i].x, data[i].y, data[i].distanceTraveled, data[i].weight);
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

    // Distribute Fish data to all processes
    if (rank == 0) {
        for (int dest = 0; dest < size; dest++) {
            if (dest != 0) {
                // Double the x and y fields before sending
                for (int i = dest * data_per_process; i < (dest + 1) * data_per_process; i++) {
                    data[i].x *= 2.0;
                    data[i].y *= 2.0;
                    // You can similarly double other fields like distanceTraveled and weight if needed.
                }
                MPI_Send(data + dest * data_per_process, data_per_process * sizeof(Fish), MPI_BYTE, dest, 0,
                         MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Recv(recv_buffer, data_per_process * sizeof(Fish), MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Double the received data before sending it back
        for (int i = 0; i < data_per_process; i++) {
            recv_buffer[i].x *= 2.0;
            recv_buffer[i].y *= 2.0;
            // You can similarly double other fields like distanceTraveled and weight if needed.
        }
        MPI_Send(recv_buffer, data_per_process * sizeof(Fish), MPI_BYTE, 0, 1, MPI_COMM_WORLD);
    }

    // Workers send Fish data back to the master
    if (rank != 0) {
        MPI_Send(recv_buffer, data_per_process * sizeof(Fish), MPI_BYTE, 0, 1, MPI_COMM_WORLD);
    } else {
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
