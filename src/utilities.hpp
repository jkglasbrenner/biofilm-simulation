#ifndef BIOFILM_SIMULATION_INDEX_CONVERTER_HPP
#define BIOFILM_SIMULATION_INDEX_CONVERTER_HPP

#include <chrono>
#include <string>

#include <cli.hpp>
#include <components.hpp>
#include <data.hpp>
#include <entities.hpp>

namespace utilities {
enum {
  kCellLocationLeftPad = 0,
  kCellLocationRightPad = 1,
  kCellLocationTopPad = 2,
  kCellLocationBottomPad = 3,
  kCellLocationFirstColumn = 4,
  kCellLocationLastColumn = 5,
  kCellLocationOther = 6,
};

std::chrono::time_point<std::chrono::high_resolution_clock> read_timer();
int count_elements(const data::Dimensions &dimensions);
int get_cell_location(data::CellLocationParameters &parameters);
void print_bacteria_grid(Entities &entities);
void print_nutrients_grid(Entities &entities);
void print_bacteria_copy_grid(Entities &entities);
void print_nutrients_copy_grid(Entities &entities);
void print_cell_locations(Entities &entities);

struct IndexConverter {
  const int kcells_per_row_;

  explicit IndexConverter(int cells_per_row);
  int operator()(const data::Index &index);
  data::Index operator()(int id);
};

bool check_if_empty(std::string filename);

void save_benchmark_to_csv(
    const cli_parameters_t &cli_parameters, double simulation_time, std::string run_mode);

void save_history_to_csv(
    std::ofstream &f, const cli_parameters_t &cli_parameters, int step, int num_entities,
    const Entities &entities);
}   // namespace utilities

#endif   // BIOFILM_SIMULATION_INDEX_CONVERTER_HPP
