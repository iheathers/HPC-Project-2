////////#include <stdio.h>
////////#include <omp.h>
////////#include <mpi.h>
////////
////////int main(int argc, char *argv[]) {
////////    FILE *fp1, *fp2;
////////    int process_id, number_of_processes;
////////
////////    MPI_Init(&argc, &argv);
////////    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
////////    MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);
////////
////////#pragma omp parallel
////////    {
////////        printf("Hello world, I am process %d among %d processes and thread_id %d among %d threads\n",
////////               process_id, number_of_processes, omp_get_thread_num(), omp_get_num_threads());
////////
////////        if ((process_id == 0) && (omp_get_thread_num() == 0)) {
////////#pragma omp critical
////////            {
////////                fp1 = fopen("out1.txt", "w+");
////////                fp2 = fopen("out2.txt", "w+");
////////                fprintf(fp1, "I am thread %d of process %d\n", omp_get_thread_num(), process_id);
////////                fprintf(fp2, "I am thread %d of process %d\n", omp_get_thread_num(), process_id);
////////                fclose(fp1);
////////                fclose(fp2);
////////            }
////////        }
////////    }
////////
////////    MPI_Finalize();
////////}
//////
//////
//////#include <stdio.h>
//////#include <stdlib.h>
//////#include <mpi.h>
//////
//////typedef struct {
//////    double x, y;
//////    double distanceTraveled;
//////    double weight;
//////} Fish;
//////
//////int main(int argc, char *argv[]) {
//////    int process_id, number_of_processes;
//////    MPI_Init(&argc, &argv);
//////    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
//////    MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);
//////
//////    // Define the fish data structure
//////    Fish *allFish = NULL;
//////    Fish *localFish = (Fish *) malloc(sizeof(Fish));
//////
//////    // Generate fish data only in the master process
//////    if (process_id == 0) {
//////        allFish = (Fish *) malloc(number_of_processes * sizeof(Fish));
//////        for (int i = 0; i < number_of_processes; i++) {
//////            allFish[i].x = 0.0;
//////            allFish[i].y = 0.0;
//////            allFish[i].distanceTraveled = 0.0;
//////            allFish[i].weight = 1.0;
//////        }
//////    }
//////
//////    // Scatter the fish data to all processes, including the master
//////    MPI_Scatter(allFish, sizeof(Fish), MPI_BYTE, localFish, sizeof(Fish), MPI_BYTE, 0, MPI_COMM_WORLD);
//////
//////    // Simulate some work (you mentioned there's no computation, so this is just a placeholder)
//////    // ...
//////
//////    // Gather the local fish data back to the master process
//////    MPI_Gather(localFish, sizeof(Fish), MPI_BYTE, allFish, sizeof(Fish), MPI_BYTE, 0, MPI_COMM_WORLD);
//////
//////    // Open and write the fish data to a file in the master process
//////    if (process_id == 0) {
//////        FILE *initialFile = fopen("initial_fish_data.txt", "w");
//////        FILE *finalFile = fopen("final_fish_data.txt", "w");
//////
//////        fprintf(initialFile, "Initial Fish Data:\n");
//////        for (int i = 0; i < number_of_processes; i++) {
//////            fprintf(initialFile, "x: %f, y: %f, distance: %f, weight: %f\n",
//////                    allFish[i].x, allFish[i].y, allFish[i].distanceTraveled, allFish[i].weight);
//////        }
//////
//////        fprintf(finalFile, "Final Fish Data:\n");
//////        for (int i = 0; i < number_of_processes; i++) {
//////            fprintf(finalFile, "x: %f, y: %f, distance: %f, weight: %f\n",
//////                    allFish[i].x, allFish[i].y, allFish[i].distanceTraveled, allFish[i].weight);
//////        }
//////
//////        fclose(initialFile);
//////        fclose(finalFile);
//////
//////        // Clean up memory allocated for allFish
//////        free(allFish);
//////    }
//////
//////    free(localFish);
//////    MPI_Finalize();
//////    return 0;
//////}
////
////
////#include <stdio.h>
////#include <stdlib.h>
////#include <time.h>
////#include <mpi.h>
////
////typedef struct {
////    double x, y;
////    double distanceTraveled;
////    double weight;
////} Fish;
////
////double rand_double() {
////    return (double) rand() / RAND_MAX;
////}
////
////void writeFishDataToFile(const char *filename, const Fish *fish, int numFish) {
////    FILE *file = fopen(filename, "w");
////    if (file == NULL) {
////        fprintf(stderr, "Error opening file: %s\n", filename);
////        return;
////    }
////
////    fprintf(file, "Fish Data:\n");
////    for (int i = 0; i < numFish; i++) {
////        fprintf(file, "x: %f, y: %f, distance: %f, weight: %f\n",
////                fish[i].x, fish[i].y, fish[i].distanceTraveled, fish[i].weight);
////    }
////
////    fclose(file);
////}
////
////int main(int argc, char *argv[]) {
////    int process_id, number_of_processes;
////    MPI_Init(&argc, &argv);
////    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
////    MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);
////
////    srand((unsigned int) time(NULL));  // Initialize random number generator
////
////    // Define the fish data structure
////    Fish *allFish = NULL;
////    Fish *localFish = (Fish *) malloc(sizeof(Fish));
////
////    // Generate random fish data in the master process
////    if (process_id == 0) {
////        allFish = (Fish *) malloc(number_of_processes * sizeof(Fish));
////        for (int i = 0; i < number_of_processes; i++) {
////            allFish[i].x = rand_double() * 10.0;  // Adjust range as needed
////            allFish[i].y = rand_double() * 10.0;  // Adjust range as needed
////            allFish[i].distanceTraveled = rand_double() * 100.0;  // Adjust range as needed
////            allFish[i].weight = rand_double() * 5.0;  // Adjust range as needed
////        }
////
////        // Write the data immediately after generation
////        writeFishDataToFile("initial_fish_data.txt", allFish, number_of_processes);
////    }
////
////    // Scatter the fish data to all processes, including the master
////    MPI_Scatter(allFish, sizeof(Fish), MPI_BYTE, localFish, sizeof(Fish), MPI_BYTE, 0, MPI_COMM_WORLD);
////
////    // Simulate some work (you mentioned there's no computation, so this is just a placeholder)
////    // ...
////
////    // Gather the local fish data back to the master process
////    MPI_Gather(localFish, sizeof(Fish), MPI_BYTE, allFish, sizeof(Fish), MPI_BYTE, 0, MPI_COMM_WORLD);
////
////    // Open and write the fish data to a file in the master process after gathering
////    if (process_id == 0) {
////        writeFishDataToFile("final_fish_data.txt", allFish, number_of_processes);
////
////        // Clean up memory allocated for allFish
////        free(allFish);
////    }
////
////    free(localFish);
////    MPI_Finalize();
////    return 0;
////}
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//#include <mpi.h>
//
//#define NUM_FISHES 8
//
//typedef struct {
//    double x, y;
//    double distanceTraveled;
//    double weight;
//} Fish;
//
//double rand_double() {
//    return (double) rand() / RAND_MAX;
//}
//
//void writeFishDataToFile(const char *filename, const Fish *fish, int numFish) {
//    FILE *file = fopen(filename, "w");
//    if (file == NULL) {
//        fprintf(stderr, "Error opening file: %s\n", filename);
//        return;
//    }
//
//    fprintf(file, "Fish Data:\n");
//    for (int i = 0; i < numFish; i++) {
//        fprintf(file, "x: %f, y: %f, distance: %f, weight: %f\n",
//                fish[i].x, fish[i].y, fish[i].distanceTraveled, fish[i].weight);
//    }
//
//    fclose(file);
//}
//
//int main(int argc, char *argv[]) {
//    int process_id, number_of_processes;
//    MPI_Init(&argc, &argv);
//    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
//    MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);
//
//    if (NUM_FISHES % number_of_processes != 0) {
//        if (process_id == 0) {
//            fprintf(stderr, "NUM_FISHES must be divisible by the number of processes.\n");
//        }
//        MPI_Finalize();
//        return 1;
//    }
//
//    srand((unsigned int) time(NULL));  // Initialize random number generator
//
//    // Define the fish data structure
//    Fish *allFish = NULL;
//    Fish *localFish = (Fish *) malloc(sizeof(Fish));
//
//    // Generate random fish data only in the master process
//    if (process_id == 0) {
//        allFish = (Fish *) malloc(NUM_FISHES * sizeof(Fish));
//        for (int i = 0; i < NUM_FISHES; i++) {
//            allFish[i].x = rand_double() * 10.0;  // Adjust range as needed
//            allFish[i].y = rand_double() * 10.0;  // Adjust range as needed
//            allFish[i].distanceTraveled = rand_double() * 100.0;  // Adjust range as needed
//            allFish[i].weight = rand_double() * 5.0;  // Adjust range as needed
//        }
//
//        // Write the data immediately after generation
//        writeFishDataToFile("initial_fish_data.txt", allFish, NUM_FISHES);
//    }
//
//    // Scatter the fish data to all processes, including the master
//    MPI_Scatter(allFish, NUM_FISHES / number_of_processes * sizeof(Fish), MPI_BYTE, localFish, sizeof(Fish), MPI_BYTE,
//                0, MPI_COMM_WORLD);
//
//    // Simulate some work (you mentioned there's no computation, so this is just a placeholder)
//    // ...
//
//    // Gather the local fish data back to the master process
//    MPI_Gather(localFish, sizeof(Fish), MPI_BYTE, allFish, NUM_FISHES / number_of_processes * sizeof(Fish), MPI_BYTE, 0,
//               MPI_COMM_WORLD);
//
//    // Open and write the fish data to a file in the master process after gathering
//    if (process_id == 0) {
//        writeFishDataToFile("final_fish_data.txt", allFish, NUM_FISHES);
//
//        // Clean up memory allocated for allFish
//        free(allFish);
//    }
//
//    free(localFish);
//    MPI_Finalize();
//    return 0;
//}


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define NUM_FISHES 16

typedef struct {
    double x, y;
    double distanceTraveled;
    double weight;
} Fish;

double rand_double() {
    return (double) rand() / RAND_MAX;
}

void writeFishDataToFile(const char *filename, const Fish *fish, int numFish) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return;
    }

    fprintf(file, "Fish Data:\n");
    for (int i = 0; i < numFish; i++) {
        fprintf(file, "x: %f, y: %f, distance: %f, weight: %f\n",
                fish[i].x, fish[i].y, fish[i].distanceTraveled, fish[i].weight);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    int process_id, number_of_processes;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
    MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);

    if (NUM_FISHES % number_of_processes != 0) {
        if (process_id == 0) {
            fprintf(stderr, "NUM_FISHES must be divisible by the number of processes.\n");
        }
        MPI_Finalize();
        return 1;
    }

    srand((unsigned int) time(NULL));  // Initialize random number generator

    // Define the fish data structure
    Fish *allFish = NULL;
    Fish *localFish = (Fish *) malloc((NUM_FISHES / number_of_processes) * sizeof(Fish));

    // Generate random fish data only in the master process
    if (process_id == 0) {
        allFish = (Fish *) malloc(NUM_FISHES * sizeof(Fish));
        for (int i = 0; i < NUM_FISHES; i++) {
            allFish[i].x = rand_double() * 10.0;  // Adjust range as needed
            allFish[i].y = rand_double() * 10.0;  // Adjust range as needed
            allFish[i].distanceTraveled = rand_double() * 100.0;  // Adjust range as needed
            allFish[i].weight = rand_double() * 5.0;  // Adjust range as needed
        }

        // Write the data immediately after generation
        writeFishDataToFile("initial_fish_data.txt", allFish, NUM_FISHES);
    }

    // Scatter the fish data to all processes, including the master
    MPI_Scatter(allFish, (NUM_FISHES / number_of_processes) * sizeof(Fish), MPI_BYTE, localFish,
                (NUM_FISHES / number_of_processes) * sizeof(Fish), MPI_BYTE, 0, MPI_COMM_WORLD);

    // Simulate some work (you mentioned there's no computation, so this is just a placeholder)
    // ...

    // Gather the local fish data back to the master process
    MPI_Gather(localFish, (NUM_FISHES / number_of_processes) * sizeof(Fish), MPI_BYTE, allFish,
               (NUM_FISHES / number_of_processes) * sizeof(Fish), MPI_BYTE, 0, MPI_COMM_WORLD);

    // Open and write the fish data to a file in the master process after gathering
    if (process_id == 0) {
        writeFishDataToFile("final_fish_data.txt", allFish, NUM_FISHES);

        // Clean up memory allocated for allFish
        free(allFish);
    }

    free(localFish);
    MPI_Finalize();
    return 0;
}
