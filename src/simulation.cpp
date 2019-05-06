#include <simulation.hpp>

#include <fstream>
#include <iostream>
#include <memory>
#include <random>

#include <cli.hpp>
#include <consumption_system.hpp>
#include <copy_system.hpp>
#include <data.hpp>
#include <diffusion_system.hpp>
#include <entities.hpp>
#include <growth_system.hpp>
#include <initializers.hpp>
#include <neighbors.hpp>
#include <utilities.hpp>

namespace serial {
void run_simulation(cli_parameters_t &cli_parameters) {
  std::mt19937 random_engine = std::mt19937(cli_parameters.seed);

  int size_padding = 1;
  int cells_per_row = cli_parameters.grid_dimensions[1] + 2 * size_padding;

  utilities::IndexConverter indexer = utilities::IndexConverter(cells_per_row);
  data::InitializationParameters parameters{
      data::Dimensions{cli_parameters.grid_dimensions[0],
                       cli_parameters.grid_dimensions[1], size_padding},
      cli_parameters.probability_bacteria, cli_parameters.max_nutrient};

  Entities entities =
      initializers::initialize_entities(parameters, indexer, random_engine);
  int num_entities = entities.names_.size();

  data::GetNeighborsParameters get_neighbors_parameters{};
  get_neighbors_parameters.cells_per_row = cells_per_row;
  get_neighbors_parameters.first_unpadded_row = size_padding;
  get_neighbors_parameters.last_unpadded_row =
      size_padding + cli_parameters.grid_dimensions[0] - 1;
  get_neighbors_parameters.wraparound_shift =
      (cli_parameters.grid_dimensions[0] - 1) * cells_per_row;

  std::ofstream history_savefile;
  utilities::save_history_to_csv(
      history_savefile, cli_parameters, 0, num_entities, entities);

  auto timer = utilities::read_timer();

  for (int step = 0; step < cli_parameters.num_steps; step++) {
    transformers::copy_bacteria_states(entities);
    transformers::copy_nutrients_states(entities);
    transformers::refresh_padded_states(entities, cells_per_row);
    transformers::apply_diffusion_transformation(
        entities, cli_parameters.diffusion_rate, get_neighbors_parameters);
    transformers::apply_growth_transformation(
        entities, cli_parameters.probability_divide, get_neighbors_parameters,
        random_engine);
    transformers::apply_consumption_transformation(
        entities, cli_parameters.consume_amount);

    utilities::save_history_to_csv(
        history_savefile, cli_parameters, step + 1, num_entities, entities);
  }

  auto simulation_time =
      std::chrono::duration<double, std::micro>(utilities::read_timer() - timer).count() /
      1e6;

  std::cout << "simulation time = " << simulation_time << " seconds"
            << "\n";

  utilities::save_benchmark_to_csv(cli_parameters, simulation_time, "serial");
}
}   // namespace serial
