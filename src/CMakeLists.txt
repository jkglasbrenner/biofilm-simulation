## IF USING CUDA: Comment first PROJECT() line, then uncomment second PROJECT()
## line.
PROJECT(biofilm VERSION 0.1.0 LANGUAGES CXX)
# PROJECT(biofilm VERSION 0.1.0 LANGUAGES CUDA CXX)

## If USING OPENMP: Uncomment line below
# FIND_PACKAGE(OpenMP REQUIRED)

## IF USING MPI: Uncomment line below
# FIND_PACKAGE(MPI REQUIRED)

## IF USING CUDA: Uncomment lines below
# IF(NOT DEFINED CMAKE_CUDA_STANDARD)
#   SET(CMAKE_CUDA_STANDARD 11)
#   SET(CMAKE_CUDA_STANDARD_REQUIRED ON)
# ENDIF()

## TODO: Adjust compiler flags_/options by editing the OPTS variable
SET(OPTS -O3)

## TODO: Add new source files to SRC_FILES list
SET(SRC_FILES cli.cpp copy_system.cpp consumption_system.cpp diffusion_system.cpp growth_system.cpp initializers.cpp neighbors.cpp simulation.cpp utilities.cpp)

## DO NOT ADJUST
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

## DO NOT ADJUST
ADD_EXECUTABLE(${PROJECT_NAME}
  ${SRC_FILES}
)

## IF USING OPENMP OR MPI: Uncomment OpenMP or MPI line as required
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
  CLI11
  # OpenMP::OpenMP_CXX
  # MPI::MPI_CXX
)

## IF USING CUDA: Uncomment TARGET_COMPILE_OPTIONS() below
# TARGET_COMPILE_OPTIONS(${PROJECT_NAME}
#   PRIVATE
#   $<$<BUILD_INTERFACE:$<COMPILE_LANGUAGE:CXX>>:${OPTS}>
#   $<$<BUILD_INTERFACE:$<COMPILE_LANGUAGE:CUDA>>:-Xcompiler=${OPTS}>
# )

## DO NOT ADJUST
INSTALL(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin/
)
