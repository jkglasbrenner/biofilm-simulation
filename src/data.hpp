#ifndef BIOFILM_SIMULATION_DATA_HPP
#define BIOFILM_SIMULATION_DATA_HPP

#include <array>

namespace data {
/// Aliases for neighbor id vectors
using ThreeNeighborIDs = std::array<int, 3>;
using FourNeighborIDs = std::array<int, 4>;
using EightNeighborIDs = std::array<int, 8>;

/// Alias for nutrients values vector
using EightNutrients = std::array<double, 8>;

/// Data container for indices component
struct Index {
  int row;      ///< Row index
  int column;   ///< Column index
};

/// Data container defining the 2D grid dimensions.
struct Dimensions {
  int number_rows;      ///< Number of unpadded rows in grid.
  int number_columns;   ///< Number of unpadded columns in grid.
  int size_padding;     ///< Width of padding wrapping the grid.
};

/// Data container of row and column indices for special locations on the padded
/// grid.
struct CellLocationParameters {
  Index index;                   ///< Row and column index of entity you are locating.
  int left_padding_boundary;     ///< Column index for the first column in the
                                 ///< regular, unpadded grid.
  int right_padding_boundary;    ///< Column index for the last column in the
                                 ///< regular, unpadded grid.
  int top_padding_boundary;      ///< Row index for the first row in the regular,
                                 ///< unpadded grid.
  int bottom_padding_boundary;   ///< Row index for the last row in the regular,
                                 ///< unpadded grid.
};

/// Data container of row and column indices and location for new entity.
struct EntityInitializationParameters {
  Index index;         ///< Row and column index of new entity.
  int cell_location;   ///< Location of new entity.
};

/// Data container of parameters used to initialize all entities in simulation.
struct InitializationParameters {
  Dimensions dimensions;          ///< Data container defining the 2D grid dimensions.
  double probability_bacterium;   ///< Probability to create a bacterium in first
                                  ///< column in the regular, unpadded grid.
  double max_nutrient;            ///< Initial fill value for nutrients component
};

/// Data container with parameters used for finding neighboring sites.
struct GetNeighborsParameters {
  int cells_per_row;        ///< The number of columns there are per row in the padded
                            ///< grid.
  int first_unpadded_row;   ///< Row index of the first row in the regular,
                            ///< unpadded grid.
  int last_unpadded_row;    ///< Row index of the last row in the regular, unpadded
                            ///< grid.
  int wraparound_shift;     ///< Row index shift needed to go from first row to last
                            ///< row, and vice-versa.
};

/// Data container with values needed for applying the diffusion transformation.
struct NutrientNeighborhood {
  double site;                ///< Nutrients value for entity at center of Moore
                              ///< neighborhood.
  EightNutrients neighbors;   ///< Array of nutrient values in Moore neighborhood.
};
}   // namespace data

#endif   // BIOFILM_SIMULATION_DATA_HPP
