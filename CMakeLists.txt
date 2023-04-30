cmake_minimum_required(VERSION 3.22)
project(Proyecto2)
enable_language(Fortran)

set(CMAKE_CXX_STANDARD 20)

find_package(MPI REQUIRED)
find_package(OpenSSL REQUIRED)

add_definitions(-DOMPI_SKIP_MPICXX)


link_libraries(MPI::MPI_CXX)
link_libraries(OpenSSL::SSL)

add_executable(Lab4 main.cpp crypto.cpp)
add_executable(test test.cpp crypto.cpp)
