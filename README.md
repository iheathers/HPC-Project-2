# High Performance Computing Project 2 - Fish Simulation

## Overview

This project is part of the CITS5507 High Performance Computing course, focusing on simulating the behavior of fish in a 2D lake environment. The simulation is implemented using MPI (Message Passing Interface) and OpenMP (Open Multi-Processing) to achieve parallelism at both inter-process and intra-process levels. The project demonstrates different approaches to parallelization, including point-to-point communication and collective communication, to efficiently simulate and analyze fish movements.

## Project Structure

  - `fish-communication-isend.c`: Implements non-blocking communication using `MPI_Isend` to avoid deadlocks.
  - `mpi-fish-communication.c`: The initial implementation using blocking MPI communication.
  - `mpi-fish-simulation-scatter-gather.c`: An implementation using collective communication with `MPI_Scatterv` and `MPI_Gatherv`.
  - `final-fish-communication.c`: Final implementation combining both MPI and OpenMP for fish simulation.
  - Other `.c` files contain different iterations and experimental code related to fish simulation and communication.
  - `fish_data_initial.txt`: Initial state of the fish before simulation.
  - `fish_data_final.txt`: Final state of the fish after simulation.
  - `fish_data_received_*.txt`: Data received by each process after distribution.

- **CMakeLists.txt**: CMake configuration file for building the project.

- **report.md**: Detailed report explaining the project, methodologies, and results.

- **Deliverable Report.md**: A summary of deliverables for the project.

## Key Features

- **Parallelism with MPI and OpenMP**: The project uses MPI for inter-process communication and OpenMP for parallelizing computations within each process.
  
- **Non-Blocking Communication**: Implements non-blocking communication using `MPI_Isend` to improve performance and avoid deadlocks.

- **Collective Communication**: Uses `MPI_Scatterv` and `MPI_Gatherv` for efficient data distribution and collection across processes.

- **Scalability Testing**: The project includes experiments testing the scalability of the simulation across multiple nodes and threads.


## Usage

- Modify the number of processes and threads in the source code to test different configurations.
- Use the data files provided in the `data/` directory to compare simulation results.

## Results

The project demonstrates the effective use of MPI and OpenMP for parallelizing complex simulations. The final implementation efficiently handles data distribution, communication, and computation across multiple processes and threads.

## License

This project is licensed under the MIT License.

## Acknowledgments

This project was developed as part of the CITS5507 High Performance Computing course at The University of Western Australia. Special thanks to the course instructors and peers for their support and guidance.
