#ifndef BIOFILM_SIMULATION_COMPONENTS_HPP
#define BIOFILM_SIMULATION_COMPONENTS_HPP

#include <string>
#include <vector>

#include <data.hpp>

namespace component {
using Names = std::vector<std::string>;
using Bacteria = std::vector<int>;
using Nutrients = std::vector<double>;
using CellIndices = std::vector<data::Index>;
using CellLocations = std::vector<int>;
using Flags = std::vector<uint32_t>;
}   // namespace component
#endif   // BIOFILM_SIMULATION_COMPONENTS_HPP
