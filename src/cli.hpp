#ifndef BIOFILM_SIMULATION_CLI_HPP
#define BIOFILM_SIMULATION_CLI_HPP

#include <iostream>
#include <string>
#include <vector>

struct cli_parameters_t {
  std::vector<int> grid_dimensions;
  std::string summary_filename;
  std::string output_filename;
  uint32_t seed;
  int num_steps;
  double consume_amount;
  double diffusion_rate;
  double probability_bacteria;
  double probability_divide;
  double max_nutrient;

  void print_parameters() {
    std::cout << "time steps                     = " << num_steps << "\n"
              << "initial bacteria probability   = " << probability_bacteria << "\n"
              << "initial nutrient grid value    = " << max_nutrient << "\n"
              << "grid dimensions                = " << grid_dimensions[0] << " rows, "
              << grid_dimensions[1] << " columns\n"
              << "bacteria division probability  = " << probability_divide << "\n"
              << "diffusion rate                 = " << diffusion_rate << "\n"
              << "consume amount                 = " << consume_amount << "\n"
              << "rng seed                       = " << seed << "\n"
              << "benchmark summary filename     = " << summary_filename << "\n"
              << "simulation history filename    = " << output_filename << "\n";
  }
};
#endif   // BIOFILM_SIMULATION_CLI_HPP
