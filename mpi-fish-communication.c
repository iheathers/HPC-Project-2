#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
// Include the time.h library for time functions
#include <time.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int num_fish = 8; // Change this value as needed
    double *fish_data = NULL;
    double *gathered_fish_data = (double *) malloc(num_fish * 3 * sizeof(double));

    if (rank == 0) {
        fish_data = (double *) malloc(num_fish * 3 * size * sizeof(double));

        // Use a fixed seed for random number generation
        srand(123);  // Change the seed value as needed

        for (int i = 0; i < num_fish * 3 * size; i++) {
            fish_data[i] = (double) rand() / RAND_MAX; // Replace with your data generation logic
        }
    }

    MPI_Scatter(fish_data, num_fish * 3, MPI_DOUBLE, gathered_fish_data, num_fish * 3, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Gather(gathered_fish_data, num_fish * 3, MPI_DOUBLE, fish_data, num_fish * 3, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        FILE *file1 = fopen("fish_data_before.txt", "w");
        for (int i = 0; i < num_fish; i++) {
            fprintf(file1, "%f %f %f\n", gathered_fish_data[i], gathered_fish_data[i + num_fish],
                    gathered_fish_data[i + 2 * num_fish]);
        }
        fclose(file1);

        FILE *file2 = fopen("fish_data_after.txt", "w");
        for (int i = 0; i < num_fish * size; i++) {
            fprintf(file2, "%f %f %f\n", fish_data[i], fish_data[i + num_fish * size],
                    fish_data[i + 2 * num_fish * size]);
        }
        fclose(file2);

        free(fish_data);
    }

    free(gathered_fish_data);

    MPI_Finalize();
    return 0;
}
