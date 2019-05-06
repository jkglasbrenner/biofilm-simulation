#include <growth_system.hpp>

#include <random>

#include <entities.hpp>
#include <neighbors.hpp>
#include <utilities.hpp>

namespace transformers {
void apply_growth_transformation(
    Entities &entities, double probability_bacterium,
    const data::GetNeighborsParameters &get_neighbors_parameters,
    std::mt19937 &random_engine) {
  std::uniform_real_distribution<double> uniform_rng(0.0, 1.0);
  int num_entities = entities.names_.size();
  uint32_t transform_flag = Entities::kFlagBacteria | Entities::kFlagBacteriaCopy |
                            Entities::kFlagNutrients | Entities::kFlagNutrientsCopy;

  for (Entities::EntityID id = 0; id < num_entities; id++) {
    uint32_t flag_check = transform_flag & entities.flags_[id];
    if (flag_check == transform_flag) {
      double current_nutrients = entities.nutrients_[id];
      bool bacterium_alive = entities.bacteria_copy_[id] == Entities::kStateBacterium;
      bool no_nutrients_left = current_nutrients < 1.0E-5;
      bool has_empty_neighbor =
          neighbors::check_for_empty_neighbors(id, entities, get_neighbors_parameters);

      if (bacterium_alive & no_nutrients_left) {
        entities.bacteria_[id] = Entities::kStateDead;
      } else if (bacterium_alive & has_empty_neighbor) {
        if (uniform_rng(random_engine) < (current_nutrients * probability_bacterium)) {
          int neighbor_id = neighbors::pick_bacterium_neighbor(
              id, entities, get_neighbors_parameters, random_engine);
          entities.bacteria_[neighbor_id] = Entities::kStateBacterium;
        }
      }
    }
  }
}
}   // namespace transformers
