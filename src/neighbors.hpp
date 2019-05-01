#ifndef BIOFILM_SIMULATION_INDEXER_SYSTEM_HPP
#define BIOFILM_SIMULATION_INDEXER_SYSTEM_HPP

#include <array>
#include <random>

#include <data.hpp>
#include <entities.hpp>
#include <utilities.hpp>

namespace neighbors {
data::ThreeNeighborIDs get_first_column_bacterium_neighbors(
    Entities::EntityID id, const Entities &entities,
    const data::GetNeighborsParameters &parameters);
data::ThreeNeighborIDs get_last_column_bacterium_neighbors(
    Entities::EntityID id, const Entities &entities,
    const data::GetNeighborsParameters &parameters);
data::FourNeighborIDs get_bacterium_neighbors(
    Entities::EntityID id, const Entities &entities,
    const data::GetNeighborsParameters &parameters);
data::EightNeighborIDs get_nutrient_neighbor_ids(
    Entities::EntityID id, const Entities &entities,
    const data::GetNeighborsParameters &parameters);
data::EightNutrients get_nutrient_neighbor_values(
    const Entities &entities, const data::EightNeighborIDs &neighbor_ids);
bool check_for_empty_neighbors(
    Entities::EntityID id, const Entities &entities,
    const data::GetNeighborsParameters &parameters);
int sample_random_id(int num_neighbors, std::mt19937 &random_engine);
Entities::EntityID pick_bacterium_neighbor(
    Entities::EntityID id, const Entities &entities,
    const data::GetNeighborsParameters &parameters, std::mt19937 &random_engine);
}   // namespace neighbors

#endif   // BIOFILM_SIMULATION_INDEXER_SYSTEM_HPP
