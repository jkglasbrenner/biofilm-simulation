#include <utilities.hpp>

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include <cli.hpp>
#include <components.hpp>
#include <data.hpp>

namespace utilities {
/// Read and return current time.
///
/// Used for benchmarking the simulation time.
///
/// @return Current time of computer's internal clock.
std::chrono::time_point<std::chrono::high_resolution_clock> read_timer() {
  auto get_time = std::chrono::high_resolution_clock::now();
  return get_time;
}

/// Count the total number of elements in 2D padded grid.
///
/// @param [in] dimensions Struct with number of rows and columns in unpadded
///   grid.
/// @return Total number of elements in 2D padded grid.
int count_elements(const data::Dimensions &dimensions) {
  int number_padded_rows = dimensions.number_rows + 2 * dimensions.size_padding;
  int number_padded_columns = dimensions.number_columns + 2 * dimensions.size_padding;

  return number_padded_rows * number_padded_columns;
}

/// Label the entity's location in 2D grid.
///
/// The possible location labels are as follows,
/// * LeftPad: Padded region, left-most column
/// * RightPad: Padded region, right-most column
/// * TopPad: Padded region, top row
/// * BottomPad: Padded region, bottom row
/// * FirstColumn: First column of main grid
/// * LastColumn: Last column of main grid
/// * Other: Cells in main grid but not in first or last column.
///
/// @param [in] parameters Struct defining the row and column indices for the
///   different grid locations.
/// @return Integer representing cell location label for entity.
int get_cell_location(data::CellLocationParameters &parameters) {
  if (parameters.index.column < parameters.left_padding_boundary) {
    return kCellLocationLeftPad;
  } else if (parameters.index.column > parameters.right_padding_boundary) {
    return kCellLocationRightPad;
  } else if (parameters.index.row < parameters.top_padding_boundary) {
    return kCellLocationTopPad;
  } else if (parameters.index.row > parameters.bottom_padding_boundary) {
    return kCellLocationBottomPad;
  } else if (parameters.index.column == parameters.left_padding_boundary) {
    return kCellLocationFirstColumn;
  } else if (parameters.index.column == parameters.right_padding_boundary) {
    return kCellLocationLastColumn;
  } else {
    return kCellLocationOther;
  }
}

/// Print the bacteria component to stdout as a 2D grid.
///
/// This is a helper function that can be used for debugging purposes. By
/// default it is not used anywhere in the program.
///
/// @param [in] entities Entities struct with components defining the
///   nutrients and bacteria grids.
void print_bacteria_grid(Entities &entities) {
  int number_entities = entities.names_.size();
  int previous_column_index = 0;

  for (int id = 0; id < number_entities; id++) {
    int column_index = entities.indices_[id].column;
    bool not_pad_cell =
        (entities.flags_[id] & Entities::kFlagBacteria) == Entities::kFlagBacteria;

    if (not_pad_cell & (column_index > previous_column_index)) {
      std::cout << " ";
    } else if (column_index < previous_column_index) {
      std::cout << "\n";
    }

    if (not_pad_cell) {
      std::cout << entities.bacteria_[id];
    }

    previous_column_index = column_index;
  }

  std::cout << "\n";
}

/// Print the nutrients component to stdout as a 2D grid.
///
/// This is a helper function that can be used for debugging purposes. By
/// default it is not used anywhere in the program.
///
/// @param [in] entities Entities struct with components defining the
///   nutrients and bacteria grids.
void print_nutrients_grid(Entities &entities) {
  int number_entities = entities.names_.size();
  int previous_column_index = 0;

  for (int id = 0; id < number_entities; id++) {
    int column_index = entities.indices_[id].column;
    bool not_pad_cell =
        (entities.flags_[id] & Entities::kFlagNutrients) == Entities::kFlagNutrients;

    if (not_pad_cell & (column_index > previous_column_index)) {
      std::cout << " ";
    } else if (column_index < previous_column_index) {
      std::cout << "\n";
    }

    if (not_pad_cell) {
      std::cout << entities.nutrients_[id];
    }

    previous_column_index = column_index;
  }

  std::cout << "\n";
}

/// Print the bacteria_copy component to stdout as a 2D grid.
///
/// This is a helper function that can be used for debugging purposes. By
/// default it is not used anywhere in the program.
///
/// @param [in] entities Entities struct with components defining the
///   nutrients and bacteria grids.
void print_bacteria_copy_grid(Entities &entities) {
  int number_entities = entities.names_.size();
  int previous_column_index = 0;

  for (int id = 0; id < number_entities; id++) {
    int column_index = entities.indices_[id].column;

    if (column_index > previous_column_index) {
      std::cout << " ";
    } else if (column_index < previous_column_index) {
      std::cout << "\n";
    }

    std::cout << entities.bacteria_copy_[id];
    previous_column_index = column_index;
  }

  std::cout << "\n";
}

/// Print the nutrients_copy component to stdout as a 2D grid.
///
/// This is a helper function that can be used for debugging purposes. By
/// default it is not used anywhere in the program.
///
/// @param [in] entities Entities struct with components defining the
///   nutrients and bacteria grids.
void print_nutrients_copy_grid(Entities &entities) {
  int number_entities = entities.names_.size();
  int previous_column_index = 0;

  for (int id = 0; id < number_entities; id++) {
    int column_index = entities.indices_[id].column;

    if (column_index > previous_column_index) {
      std::cout << " ";
    } else if (column_index < previous_column_index) {
      std::cout << "\n";
    }

    std::cout << entities.nutrients_copy_[id];
    previous_column_index = column_index;
  }

  std::cout << "\n";
}

/// Print the locations component to stdout as a 2D grid.
///
/// This is a helper function that can be used for debugging purposes. By
/// default it is not used anywhere in the program.
///
/// @param [in] entities Entities struct with components defining the
///   nutrients and bacteria grids.
void print_cell_locations(Entities &entities) {
  int number_entities = entities.names_.size();
  int previous_column_index = 0;

  for (int id = 0; id < number_entities; id++) {
    int column_index = entities.indices_[id].column;

    if (column_index > previous_column_index) {
      std::cout << " ";
    } else if (column_index < previous_column_index) {
      std::cout << "\n";
    }

    std::cout << entities.locations_[id];
    previous_column_index = column_index;
  }

  std::cout << "\n";
}

/// IndexConverter function object constructor.
///
/// @param [in] cells_per_row The number of columns there are per row in the
///   padded grid.
IndexConverter::IndexConverter(int cells_per_row) : kcells_per_row_(cells_per_row) {}

/// Convert a (row, column) paired index into an entity id.
///
/// Overloads operator() so IndexConverter can be used as function object.
///
/// @param [in] index Row and column indices of entity.
/// @return Entity id occupying cell at (row, column)
int IndexConverter::operator()(const data::Index &index) {
  return index.column + kcells_per_row_ * index.row;
}

/// Convert an entity id into a (row, column) paired index.
///
/// Overloads operator() so IndexConverter can be used as function object.
///
/// @param [in] id Entity id.
/// @return Row and column indices of entity.
data::Index IndexConverter::operator()(int id) {
  data::Index index{};
  index.row = id / kcells_per_row_;
  index.column = id % kcells_per_row_;

  return index;
}

/// Check if a file is empty or not.
///
/// @param [in] filename Path to file to check.
/// @return Boolean indicating if file is empty.
bool check_if_empty(std::string filename) {
  std::ifstream savefile;
  savefile.open(filename);
  bool is_file_empty = savefile.peek() == std::ifstream::traits_type::eof();
  savefile.close();

  return is_file_empty;
}

/// Save benchmark results to csv format if filename is provided.
///
/// If no filename is set via the command-line interface, then no file will be
/// saved. If a filename is provided, then if the file is empty (or doesn't
/// exist), it will be created and a header line will be added. The benchmark
/// results will then be appended to the file. This allows you to run multiple
/// benchmarks and collect the results in a single file.
///
/// @param [in] cli_parameters Simulation parameters passed via the
///   command-line.
/// @param [in] simulation_time The simulation running time in seconds.
/// @param [in] run_mode Simulation running mode, i.e. serial mode.
void save_benchmark_to_csv(
    const cli_parameters_t &cli_parameters, double simulation_time,
    std::string run_mode) {
  std::ofstream benchmark_savefile;
  if (cli_parameters.summary_filename.empty()) {
    benchmark_savefile.open(nullptr);
  } else {
    benchmark_savefile.open(cli_parameters.summary_filename, std::ios::app);
  }

  if (benchmark_savefile) {
    if (check_if_empty(cli_parameters.summary_filename)) {
      benchmark_savefile << "steps"
                         << ","
                         << "num_rows"
                         << ","
                         << "num_columns"
                         << ","
                         << "sim_time"
                         << ","
                         << "run_mode"
                         << "\r\n";
    }
    benchmark_savefile << cli_parameters.num_steps << ","
                       << cli_parameters.grid_dimensions[0] << ","
                       << cli_parameters.grid_dimensions[1] << "," << simulation_time
                       << "," << run_mode << "\r\n";
  }
}

/// Save simulation history snapshots to csv format if filename is provided.
///
/// If no filename is set via the command-line interface, then no file will be
/// saved. If a filename is provided, then if the file is empty (or doesn't
/// exist), it will be created and a header line will be added. The simulation
/// history snapshots are then accumulated in the file stream buffer to be saved
/// to disk. The output file can be used for generating animations.
///
/// @param [in,out] f The file stream buffer for the output file.
/// @param [in] cli_parameters Simulation parameters passed via the
///   command-line.
/// @param [in] step The current step number in the simulation.
/// @param [in] num_entities The total number of entities in the simulation.
/// @param [in] entities Entities struct with components defining the
///   nutrients and bacteria grids.
void save_history_to_csv(
    std::ofstream &f, const cli_parameters_t &cli_parameters, int step, int num_entities,
    const Entities &entities) {
  static bool first = true;
  uint32_t real_cell = Entities::kFlagBacteria | Entities::kFlagNutrients;

  if (first) {
    if (cli_parameters.output_filename.empty()) {
      f.open(nullptr);
    } else {
      f.open(cli_parameters.output_filename);
      f << "step"
        << ","
        << "cell_id"
        << ","
        << "bacterium_state"
        << ","
        << "nutrient_state"
        << "\r\n";
    }
    first = false;
  }

  if (f) {
    int cell_id = 0;
    for (int id = 0; id < num_entities; id++) {
      if ((entities.flags_[id] & real_cell) == real_cell) {
        f << step << "," << cell_id << "," << entities.bacteria_[id] << ","
          << entities.nutrients_[id] << "\r\n";
        cell_id++;
      }
    }
  }
}
}   // namespace utilities
