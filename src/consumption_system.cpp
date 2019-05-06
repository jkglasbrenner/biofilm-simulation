#include <consumption_system.hpp>

#include <algorithm>

#include <entities.hpp>

namespace transformers {
/// Decrease nutrients component by fixed amount when bacterium is alive.
///
/// @param [in,out] entities Entities struct with components defining the
///   nutrients and bacteria grids.
/// @param [in] consumed Amount of nutrients each live bacterium consumes.
void apply_consumption_transformation(Entities &entities, double consumed) {
  // Count number of entities in simulation.
  int num_entities = entities.names_.size();

  // Mask: bacteria and nutrients components are active
  uint32_t transform_flag = Entities::kFlagBacteria | Entities::kFlagNutrients;

  // Loop over all entities regardless of active components.
  for (Entities::EntityID id = 0; id < num_entities; id++) {
    // Check if current entity's bacteria and nutrients components are active
    uint32_t flag_check = transform_flag & entities.flags_[id];
    if (flag_check == transform_flag) {
      // If active, then check if bacteria is alive
      if (entities.bacteria_[id] == Entities::kStateBacterium) {
        // If alive, then consume nutrients.
        entities.nutrients_[id] = std::max(0.0, entities.nutrients_[id] - consumed);
      }
    }
  }
}
}   // namespace transformers
