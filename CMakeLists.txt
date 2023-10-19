cmake_minimum_required(VERSION 3.26)
project(Project_2 C)

set(CMAKE_C_STANDARD 11)

# Specify the path to GCC-13 (replace with your actual path)
set(CMAKE_C_COMPILER /opt/homebrew/bin/gcc-13)

# Find MPI package
find_package(MPI REQUIRED)

# Find OpenMP package
find_package(OpenMP REQUIRED)

add_executable(Project_2


        #        fish-struct-communication.c
        #        mpi-fish-communication.c


        #        test.c
        #        scatter-demo.c
        #        mpi-fish-simulation.c
        fish-communication-isend.c
        #        scatter-demo.c
        #        mpi-fish-simuation-scatter-gather.c
        #        final-fish-co
        #        mmunication.c
        #        send-recv-squared.c
        #        hello-test.c
)

# Enable MPI support
target_include_directories(Project_2 PRIVATE ${MPI_C_INCLUDE_PATH})
target_link_libraries(Project_2 PRIVATE MPI::MPI_C)

# Enable OpenMP support
if (OpenMP_C_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    target_link_libraries(Project_2 PRIVATE ${OpenMP_C_LIBRARIES})
endif ()
