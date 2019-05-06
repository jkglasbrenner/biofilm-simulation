#ifndef BIOFILM_SIMULATION_COPY_SYSTEM_HPP
#define BIOFILM_SIMULATION_COPY_SYSTEM_HPP

#include <entities.hpp>

namespace transformers {
void refresh_padded_states(Entities &entities, int cells_per_row);
void copy_bacteria_states(Entities &entities);
void copy_nutrients_states(Entities &entities);
}   // namespace transformers

#endif   // BIOFILM_SIMULATION_COPY_SYSTEM_HPP
