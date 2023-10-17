// AUTHORS:
// PRITAM SUWAL SHRESTHA (23771397)
// RASPREET KHANUJA (23308425)

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <mpi.h>

#define LAKE_X_MIN (-100.0) // Minimum x coordinate of the lake
#define LAKE_X_MAX 100.0    // Maximum x coordinate of the lake
#define LAKE_Y_MIN (-100.0) // Minimum y coordinate of the lake
#define LAKE_Y_MAX 100.0    // Maximum y coordinate of the lake

#define MIN_SWIM_DISTANCE (-0.1) // Minimum distance a fish can swim in one step
#define MAX_SWIM_DISTANCE 0.1    // Maximum distance a fish can swim in one step

#define MAX_FISH_WEIGHT 2.0 // Maximum weight of a fish
#define MIN_FISH_WEIGHT 0.0 // Minimum weight of a fish

#define NUM_FISHES 10
#define NUM_SIMULATION_STEPS 10 // Number of simulation steps

double square(double num) { return num * num; }

typedef struct {
    double x, y;
    double distanceTraveled;
    double weight;
} Fish;

bool isFishOutsideLake(Fish *fish) {
    return (fish->x < LAKE_X_MIN || fish->x > LAKE_X_MAX ||
            fish->y < LAKE_Y_MIN || fish->y > LAKE_Y_MAX);
}

double getRandomNumberInRange(double minValue, double maxValue) {
    double randomDouble = ((double) rand() / RAND_MAX);
    double randomNumber = randomDouble * (maxValue - minValue) + minValue;
    return randomNumber;
}

double getRandomCoordinateInRange(double minValue, double maxValue) {
    return getRandomNumberInRange(minValue, maxValue);
}

double calculateDistanceFromOrigin(double x, double y) {
    return sqrt(square(x) + square(y));
}

double calculateObjectiveFunction(Fish *fishes, int numFishes) {
    double sum = 0.0;

    for (int i = 0; i < numFishes; i++) {
        sum += calculateDistanceFromOrigin(fishes[i].x, fishes[i].y);
    }

    return sum;
}

void swim(Fish *fish) {
    if (!isFishOutsideLake(fish)) {
        fish->x += getRandomNumberInRange(MIN_SWIM_DISTANCE, MAX_SWIM_DISTANCE);
        fish->y += getRandomNumberInRange(MIN_SWIM_DISTANCE, MAX_SWIM_DISTANCE);
    }
}

void updateWeight(Fish *fish, double maxDistanceTraveledInFishSchool) {
    double weightChange = fish->distanceTraveled / maxDistanceTraveledInFishSchool;
    fish->weight = fmax(MIN_FISH_WEIGHT, fmin(MAX_FISH_WEIGHT, fish->weight + weightChange));
}

void simulationStep(Fish *fishes, int numFishes) {
    double maxDistanceTraveledInFishSchool = 0.0;

    for (int i = 0; i < numFishes; i++) {
        double prevDistance = calculateDistanceFromOrigin(fishes[i].x, fishes[i].y);
        swim(&fishes[i]);
        double nextDistance = calculateDistanceFromOrigin(fishes[i].x, fishes[i].y);
        fishes[i].distanceTraveled = nextDistance - prevDistance;

        if (fabs(fishes[i].distanceTraveled) > maxDistanceTraveledInFishSchool) {
            maxDistanceTraveledInFishSchool = fabs(fishes[i].distanceTraveled);
        }
    }

    for (int i = 0; i < numFishes; i++) {
        updateWeight(&fishes[i], maxDistanceTraveledInFishSchool);
    }
}

void calculateBarycenter(Fish *fishes, int numFishes) {
    double weightSum = 0.0;
    double distanceSum = 0.0;

    for (int i = 0; i < numFishes; i++) {
        weightSum += fishes[i].weight * calculateDistanceFromOrigin(fishes[i].x, fishes[i].y);
        distanceSum += calculateDistanceFromOrigin(fishes[i].x, fishes[i].y);
    }

    if (distanceSum < 1e-6) {
        printf("Distance sum is zero. Cannot calculate barycenter.\n");
        return;
    }

    double barycenter = weightSum / distanceSum;
    printf("Barycenter: %.2f\n", barycenter);
}

void initializeInitialLakeState(Fish *fishes, int numFishes) {
    for (int i = 0; i < numFishes; i++) {
        fishes[i].x = getRandomCoordinateInRange(LAKE_X_MIN, LAKE_X_MAX);
        fishes[i].y = getRandomCoordinateInRange(LAKE_Y_MIN, LAKE_Y_MAX);
        fishes[i].distanceTraveled = 0.0;
        fishes[i].weight = 1.0;
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int numProcesses, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(time(NULL) + rank);

    int numFishesPerProcess = NUM_FISHES / numProcesses;
    Fish *fishes = (Fish *) malloc(numFishesPerProcess * sizeof(Fish));

    if (fishes == NULL) {
        fprintf(stderr, "Memory allocation failed. Exiting...\n");
        MPI_Finalize();
        return 1;
    }

    initializeInitialLakeState(fishes, numFishesPerProcess);

    double startTime = omp_get_wtime();

    for (int step = 0; step < NUM_SIMULATION_STEPS; step++) {
        printf("Simulation Step: %d (Process %d)\n", step, rank);
        printf("##################################################\n");
        simulationStep(fishes, numFishesPerProcess);
        calculateObjectiveFunction(fishes, numFishesPerProcess);
        calculateBarycenter(fishes, numFishesPerProcess);
        printf("--------------------------------------------------\n");
    }

    double endTime = omp_get_wtime();
    double time_taken = endTime - startTime;
    printf("Elapsed time: %.4f seconds (Process %d)\n", time_taken, rank);

    double *objectiveValues = (double *) malloc(numProcesses * sizeof(double));
    MPI_Gather(&time_taken, 1, MPI_DOUBLE, objectiveValues, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double finalObjectiveValue = 0.0;
        for (int i = 0; i < numProcesses; i++) {
            finalObjectiveValue += objectiveValues[i];
        }
        printf("Final Objective Function Value: %.2f\n", finalObjectiveValue);
    }

    free(fishes);
    free(objectiveValues);
    MPI_Finalize();

    return 0;
}
