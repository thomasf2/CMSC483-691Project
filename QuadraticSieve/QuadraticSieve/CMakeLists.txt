# CMakeList.txt : CMake project for TrialDivision, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# add_definitions( -DSHOW_STATUS_MESSAGES )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}_${CMAKE_BUILD_TYPE})


find_package(GMP REQUIRED)

# add_definitions(-DSHOW_STATUS_MESSAGES)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(MPFR REQUIRED)
include_directories(${MPFR_INCLUDE_DIR})

find_package(OpenMP)

set(LIBS ${LIBS} gmp gmpxx Threads::Threads ${MPFR_LIBRARIES} OpenMP::OpenMP_C)

file(GLOB THE_SOURCE
	"src/**/*.cpp"
	"src/*.cpp"
	"src/*.c"
    "main.cpp"
)
message(STATUS "SRC is ${THE_SOURCE}")
message(STATUS "Libraries are: ${LIBS}")

# Add source to this project's executable.
add_executable (qs ${THE_SOURCE})


target_link_libraries(qs ${LIBS})

# TODO: Add tests and install targets if needed.
