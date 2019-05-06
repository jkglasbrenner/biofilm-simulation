#include <growth_system.hpp>

#include <random>

#include <entities.hpp>
#include <neighbors.hpp>
#include <utilities.hpp>

namespace transformers {
/// Grow new bacteria through cellular division if nutrients are available.
///
/// @param [in,out] entities Entities struct with components defining the
///   nutrients and bacteria grids.
/// @param [in] probability_division Probability that a cell will divide
///   (scales nutrients value).
/// @param [in] get_neighbors_parameters Input data used for finding neighboring
///   sites.
/// @param [in,out] random_engine Mersenne Twister engine for random number
/// generation.
void apply_growth_transformation(
    Entities &entities, double probability_division,
    const data::GetNeighborsParameters &get_neighbors_parameters,
    std::mt19937 &random_engine) {
  // Uniform random number generator for range [0.0, 1.0)
  std::uniform_real_distribution<double> uniform_rng(0.0, 1.0);

  // Count number of entities in simulation.
  int num_entities = entities.names_.size();

  // Mask: bacteria, bacteria_copy, nutrients, and nutrients_copy components are
  // active
  uint32_t transform_flag = Entities::kFlagBacteria | Entities::kFlagBacteriaCopy |
                            Entities::kFlagNutrients | Entities::kFlagNutrientsCopy;

  // Loop over all entities regardless of active components.
  for (Entities::EntityID id = 0; id < num_entities; id++) {
    // Check if current entity's bacteria, bacteria_copy, nutrients, and
    // nutrients_copy components are active
    uint32_t flag_check = transform_flag & entities.flags_[id];
    if (flag_check == transform_flag) {
      // If active, read current nutrients (this is post-diffusion)
      double current_nutrients = entities.nutrients_[id];

      // Check is bacteria is alive
      bool bacterium_alive = entities.bacteria_copy_[id] == Entities::kStateBacterium;

      // Check if nutrients have run out
      bool no_nutrients_left = current_nutrients < 1.0E-5;

      // Check for at least one empty neighbor in bacteria grid
      bool has_empty_neighbor =
          neighbors::check_for_empty_neighbors(id, entities, get_neighbors_parameters);

      // If bacteria is alive but has no nutrients...
      if (bacterium_alive & no_nutrients_left) {
        // The bacteria dies
        entities.bacteria_[id] = Entities::kStateDead;

        // If the bacteria is alive, has nutrients, and an empty neighbor
        // cell...
      } else if (bacterium_alive & has_empty_neighbor) {
        // Sample a random number and check if bacteria will divide
        if (uniform_rng(random_engine) < (current_nutrients * probability_division)) {
          // If bacteria will divide, then pick a random empty cell for
          // expansion
          int neighbor_id = neighbors::pick_bacterium_neighbor(
              id, entities, get_neighbors_parameters, random_engine);
          // and insert live bacterium into empty cell that was picked
          entities.bacteria_[neighbor_id] = Entities::kStateBacterium;
        }
      }
    }
  }
}
}   // namespace transformers
