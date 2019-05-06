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
/// Run serial version of biofilm simulation
///
/// @param [in] cli_parameters Simulation parameters passed via the command-line
///   interface.
void run_simulation(cli_parameters_t &cli_parameters) {
  // Initialize Mersenne Twister random engine
  std::mt19937 random_engine = std::mt19937(cli_parameters.seed);

  // Padding region (ghost cells) thickness fixed to width of one cell
  int size_padding = 1;

  //
  // Use command-line parameters to compute other simulation parameters
  //
  int cells_per_row = cli_parameters.grid_dimensions[1] + 2 * size_padding;

  // Input parameters for creating and initializing simulation entities.
  data::InitializationParameters parameters{
      data::Dimensions{cli_parameters.grid_dimensions[0],
                       cli_parameters.grid_dimensions[1], size_padding},
      cli_parameters.probability_bacteria, cli_parameters.max_nutrient};

  // Input parameters needed for finding and picking neighbors.
  data::GetNeighborsParameters get_neighbors_parameters{};
  get_neighbors_parameters.cells_per_row = cells_per_row;
  get_neighbors_parameters.first_unpadded_row = size_padding;
  get_neighbors_parameters.last_unpadded_row =
      size_padding + cli_parameters.grid_dimensions[0] - 1;
  get_neighbors_parameters.wraparound_shift =
      (cli_parameters.grid_dimensions[0] - 1) * cells_per_row;

  //
  // Indexer converts 1D cell/entity id to row/column indices
  //
  utilities::IndexConverter indexer = utilities::IndexConverter(cells_per_row);

  //
  // Setup and initialize simulation entities (bacteria and nutrients cells)
  //
  Entities entities =
      initializers::initialize_entities(parameters, indexer, random_engine);

  // Number of entities created
  int num_entities = entities.names_.size();

  //
  // Save initial states to disk (if output_filename is set)
  //
  std::ofstream history_savefile;
  utilities::save_history_to_csv(
      history_savefile, cli_parameters, 0, num_entities, entities);

  //
  // Start benchmark timer
  //
  auto timer = utilities::read_timer();

  //
  // Simulate a number of time steps
  //
  for (int step = 0; step < cli_parameters.num_steps; step++) {
    //
    // Store a copy of bacteria and nutrients states from prior step
    //
    transformers::copy_bacteria_states(entities);
    transformers::copy_nutrients_states(entities);

    //
    // Refresh states in padded region (enforces boundary conditions)
    //
    transformers::refresh_padded_states(entities, cells_per_row);

    //
    // Diffusion phase: apply diffusion transformation to nutrients states
    //
    transformers::apply_diffusion_transformation(
        entities, cli_parameters.diffusion_rate, get_neighbors_parameters);

    //
    // Growth phase: apply growth transformation to bacteria states
    //
    transformers::apply_growth_transformation(
        entities, cli_parameters.probability_divide, get_neighbors_parameters,
        random_engine);

    //
    // Consume phase: apply consumption transformation to bacteria and nutrients
    // states
    //
    transformers::apply_consumption_transformation(
        entities, cli_parameters.consume_amount);

    //
    // Save current states to disk (if output_filename is set)
    //
    utilities::save_history_to_csv(
        history_savefile, cli_parameters, step + 1, num_entities, entities);
  }

  //
  // Stop benchmark timer
  //
  auto simulation_time =
      std::chrono::duration<double, std::micro>(utilities::read_timer() - timer).count() /
      1e6;

  // Print benchmark timer results to stdout.
  std::cout << "simulation time = " << simulation_time << " seconds"
            << "\n";

  //
  // Save benchmark results to disk (if summary_filename is set)
  //
  utilities::save_benchmark_to_csv(cli_parameters, simulation_time, "serial");
}
}   // namespace serial
