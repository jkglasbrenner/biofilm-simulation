#!/bin/bash
#
# Wrapper for compiling code using cmake.

###########################################################################################
# Global parameters
###########################################################################################
readonly script_dir=$(dirname ${0})
readonly project_root=$(cd ${script_dir} && pwd)
readonly mode="$1"
readonly args="${@:2}"

###########################################################################################
# Load modules for compiling on Argo
###########################################################################################

argo_load_modules () {
  if command -v module >/dev/null; then
    module load cmake/3.10.2 gcc/5.2.0 cuda/10.0 openmpi/4.0.0
  fi
}

###########################################################################################
# Compile source code
#
# Globals:
#   project_root                           # Absolute path of project root
# Arguments:
#   1: CMake build directory               # default: build"
# Returns:
#   None
###########################################################################################

compile () {
  local cmake_build_dir="${1:-build}"

  if [[ ! -d ${project_root}/${cmake_build_dir} ]]; then
    mkdir -p ${project_root}/${cmake_build_dir}
    cd ${project_root}/${cmake_build_dir}
    CC=gcc CXX=g++ cmake -DCMAKE_BUILD_TYPE=Release ../
    cmake ../
    make -j4
    make install
  else
    cd ${project_root}/${cmake_build_dir}
    make -j4
    make install
  fi

  cd ${project_root}
}

display_help () {
  echo "make.sh - Wrapper for compiling cmake-based source code"
  echo ""
  echo "Usage: make.sh [mode] [position_arguments ..]"
  echo ""
  echo "Mode:"
  echo "  argo (default)                        # Compile on Argo"
  echo "  local                                 # Compile on local machine without modules"
  echo ""
  echo "Position Arguments:"
  echo "  1: CMake build directory              # default: build"
}

###########################################################################################
# Main script
###########################################################################################

if [[ ${mode} == "argo" ]]; then
  argo_load_modules
  compile ${args}
elif [[ ${mode} == "local" ]]; then
  compile ${args}
elif [[ ${mode} == "help" || ${mode} == "--help" || ${mode} == "-h" ]]; then
  display_help
else
  display_help
fi
