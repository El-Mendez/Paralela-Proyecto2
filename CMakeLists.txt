cmake_minimum_required(VERSION 3.22)
project(Proyecto2)
enable_language(Fortran)

set(CMAKE_CXX_STANDARD 20)

find_package(MPI REQUIRED)
find_package(OpenSSL REQUIRED)

add_definitions(-DOMPI_SKIP_MPICXX)


link_libraries(MPI::MPI_CXX)
link_libraries(OpenSSL::SSL)

set(sourceCode main.cpp crypto/crypto.cpp utils/utils.cpp)

add_executable(Naive ${sourceCode} approaches/naive.cpp)
add_executable(Intercalado ${sourceCode} approaches/intercalado.cpp)
