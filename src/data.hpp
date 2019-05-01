#ifndef BIOFILM_SIMULATION_DATA_HPP
#define BIOFILM_SIMULATION_DATA_HPP

#include <array>

namespace data {
using ThreeNeighborIDs = std::array<int, 3>;
using FourNeighborIDs = std::array<int, 4>;
using EightNeighborIDs = std::array<int, 8>;
using EightNutrients = std::array<double, 8>;

struct Index {
  int row;
  int column;
};

struct Dimensions {
  int number_rows;
  int number_columns;
  int size_padding;
};

struct CellLocationParameters {
  Index index;
  int left_padding_boundary;
  int right_padding_boundary;
  int top_padding_boundary;
  int bottom_padding_boundary;
};

struct EntityInitializationParameters {
  Index index;
  int cell_location;
};

struct InitializationParameters {
  Dimensions dimensions;
  double probability_bacterium;
  double max_nutrient;
};

struct GetNeighborsParameters {
  int cells_per_row;
  int first_unpadded_row;
  int last_unpadded_row;
  int wraparound_shift;
};

struct NutrientNeighborhood {
  double site;
  EightNutrients neighbors;
};
}   // namespace data

#endif   // BIOFILM_SIMULATION_DATA_HPP
