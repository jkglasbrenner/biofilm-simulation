#include <cli.hpp>

#include <random>
#include <string>
#include <vector>

#include <CLI11.hpp>

#include <simulation.hpp>

int main(int argc, char **argv) {

  CLI::App app;
  cli_parameters_t cli_parameters;
  std::random_device rd;

  std::string mode_option = "serial";
  app.add_set_ignore_case(
      "mode", mode_option, {"serial"},
      "\n\nBiofilm simulation run modes\n"
      "  serial:   (default) Serial version of simulation.");

  cli_parameters.grid_dimensions = {400, 100};
  app.add_option(
         "-d,--dim", cli_parameters.grid_dimensions,
         "Set number of rows, columns [default: 400 100]")
      ->expected(2);

  cli_parameters.summary_filename = "";
  app.add_option(
      "-s,--summary", cli_parameters.summary_filename,
      "Save benchmark summary to csv file");

  cli_parameters.output_filename = "";
  app.add_option(
      "-o,--output", cli_parameters.output_filename,
      "Save simulation history to csv file");

  cli_parameters.num_steps = 1000;
  app.add_option(
      "-t,--time-steps", cli_parameters.num_steps,
      "Total time steps to run simulation [default: 1000]");

  cli_parameters.probability_bacteria = 0.5;
  app.add_option(
         "--prob-init", cli_parameters.probability_bacteria,
         "Probability to initialize bacteria [default: 0.5]")
      ->check(CLI::Range(0.0, 1.0));

  cli_parameters.probability_divide = 0.3;
  app.add_option(
         "--prob-divide", cli_parameters.probability_divide,
         "Probability that a bacterium divides [default: 0.3]")
      ->check(CLI::Range(0.0, 1.0));

  cli_parameters.consume_amount = 0.1;
  app.add_option(
         "-c,--consume", cli_parameters.consume_amount,
         "Nutrients bacteria consume per time step [default: 0.1]")
      ->check(CLI::Range(0.0, 1.0));

  cli_parameters.diffusion_rate = 0.1;
  app.add_option(
         "--diffusion-rate", cli_parameters.diffusion_rate,
         "Diffusion rate for nutrients [default: 0.1]")
      ->check(CLI::Range(0.0, 0.125));

  cli_parameters.max_nutrient = 1.0;
  app.add_option(
         "--max-nutrient", cli_parameters.max_nutrient,
         "Initial fill value for nutrient grid [default: 1.0]")
      ->check(CLI::Range(0.0, 1.0));

  cli_parameters.seed = rd();
  app.add_option("--seed", cli_parameters.seed, "Random number generator seed");

  CLI11_PARSE(app, argc, argv);

  std::cout << "\n"
            << "Biofilm simulation parameters"
            << "\n"
            << "-----------------------------"
            << "\n";
  cli_parameters.print_parameters();
  std::cout << "\n";

  if (mode_option == "serial") {
    serial::run_simulation(cli_parameters);
  } else {
    std::cout << mode_option << " is not a valid mode, exiting...\n";
  }

  return 0;
}
