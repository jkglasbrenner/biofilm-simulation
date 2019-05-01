#ifndef BIOFILM_SIMULATION_GROWTH_SYSTEM_HPP
#define BIOFILM_SIMULATION_GROWTH_SYSTEM_HPP

#include <random>

#include <entities.hpp>
#include <utilities.hpp>

namespace transformers {
void apply_growth_transformation(
    Entities &entities, double probability_bacterium,
    const data::GetNeighborsParameters &get_neighbors_parameters,
    std::mt19937 &random_engine);
}
#endif   // BIOFILM_SIMULATION_GROWTH_SYSTEM_HPP
