#ifndef BIOFILM_SIMULATION_DIFFUSION_SYSTEM_HPP
#define BIOFILM_SIMULATION_DIFFUSION_SYSTEM_HPP

#include <data.hpp>
#include <utilities.hpp>

namespace transformers {
void apply_diffusion_transformation(
    Entities &entities, double diffusion_rate,
    const data::GetNeighborsParameters &get_neighbors_parameters);
double
apply_site_diffusion(double diffusion_rate, data::NutrientNeighborhood &neighborhood);
}   // namespace transformers

#endif   // BIOFILM_SIMULATION_DIFFUSION_SYSTEM_HPP
