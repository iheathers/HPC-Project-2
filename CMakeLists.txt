cmake_minimum_required(VERSION 3.26)
project(Project_2 C)

set(CMAKE_C_STANDARD 11)

find_package(MPI REQUIRED)

add_executable(Project_2 mpi-fish-communication.c)

target_include_directories(Project_2 PRIVATE ${MPI_C_INCLUDE_PATH})
target_link_libraries(Project_2 PRIVATE MPI::MPI_C)
