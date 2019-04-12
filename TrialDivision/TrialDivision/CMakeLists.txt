# CMakeList.txt : CMake project for TrialDivision, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# add_definitions( -DLOG_OUTPUT )

find_package(GMP REQUIRED)
include_directories(${MPFR_INCLUDE_DIRS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(LIBS ${LIBS} gmp gmpxx Threads::Threads)

file(GLOB THE_SOURCE
	"src/**/*.h"
	"src/**/*.cpp"
	"src/*.h"
	"src/*.cpp"
    "main.cpp"
)
message(STATUS "SRC is ${THE_SOURCE}")
message(STATUS "Libraries are: ${LIBS}")

# Add source to this project's executable.
add_executable (tdiv ${THE_SOURCE})


target_link_libraries(tdiv ${LIBS})

# TODO: Add tests and install targets if needed.
