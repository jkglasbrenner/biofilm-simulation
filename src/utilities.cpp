#include <utilities.hpp>

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include <cli.hpp>
#include <components.hpp>
#include <data.hpp>

namespace utilities {
std::chrono::time_point<std::chrono::high_resolution_clock> read_timer() {
  auto get_time = std::chrono::high_resolution_clock::now();
  return get_time;
}

int count_elements(const data::Dimensions &dimensions) {
  int number_padded_rows = dimensions.number_rows + 2 * dimensions.size_padding;
  int number_padded_columns = dimensions.number_columns + 2 * dimensions.size_padding;

  return number_padded_rows * number_padded_columns;
}

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

IndexConverter::IndexConverter(int cells_per_row) : kcells_per_row_(cells_per_row) {}

int IndexConverter::operator()(const data::Index &index) {
  return index.column + kcells_per_row_ * index.row;
}

data::Index IndexConverter::operator()(int id) {
  data::Index index{};
  index.row = id / kcells_per_row_;
  index.column = id % kcells_per_row_;

  return index;
}

UnpaddedToPaddedIDConverter::UnpaddedToPaddedIDConverter(
    int unpadded_cells_per_row, int size_padding)
    : ksize_padding_(size_padding),
      unpadded_index_converter_(IndexConverter(unpadded_cells_per_row)),
      padded_index_converter_(IndexConverter(unpadded_cells_per_row + 2 * size_padding)) {
}

int UnpaddedToPaddedIDConverter::operator()(int unpadded_cell_id) {
  data::Index unpadded_index = unpadded_index_converter_(unpadded_cell_id);
  data::Index padded_index{unpadded_index.row + ksize_padding_,
                           unpadded_index.column + ksize_padding_};

  return padded_index_converter_(padded_index);
}

bool check_if_empty(std::string filename) {
  std::ifstream savefile;
  savefile.open(filename);
  bool is_file_empty = savefile.peek() == std::ifstream::traits_type::eof();
  savefile.close();

  return is_file_empty;
}

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
