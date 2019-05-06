#include <diffusion_system.hpp>

#include <data.hpp>
#include <entities.hpp>
#include <neighbors.hpp>

namespace transformers {
/// Apply diffusion transformation to nutrients component of all entities.
///
/// @param [in,out] entities Entities struct with components defining the
///   nutrients and bacteria grids.
/// @param [in] diffusion_rate Diffusion rate for nutrients.
/// @param [in] get_neighbors_parameters Input data used for finding neighboring
///   sites.
void apply_diffusion_transformation(
    Entities &entities, double diffusion_rate,
    const data::GetNeighborsParameters &get_neighbors_parameters) {
  // Count number of entities in simulation.
  int num_entities = entities.names_.size();

  // Mask: nutrients and nutrients_copy components are active
  uint32_t transform_flag = Entities::kFlagNutrients | Entities::kFlagNutrientsCopy;

  // Loop over all entities regardless of active components.
  for (Entities::EntityID id = 0; id < num_entities; id++) {
    // Check if current entity's nutrients and nutrients_copy components are
    // active
    uint32_t flag_check = transform_flag & entities.flags_[id];
    if (flag_check == transform_flag) {
      // If active, find entity ids of neighboring cells
      data::EightNeighborIDs neighbor_ids =
          neighbors::get_nutrient_neighbor_ids(id, entities, get_neighbors_parameters);

      // Package input data for diffusion transformation
      data::NutrientNeighborhood nutrient_neighborhood{};
      nutrient_neighborhood.site = entities.nutrients_copy_[id];
      nutrient_neighborhood.neighbors =
          neighbors::get_nutrient_neighbor_values(entities, neighbor_ids);

      // Apply transform and update nutrients component.
      entities.nutrients_[id] =
          apply_site_diffusion(diffusion_rate, nutrient_neighborhood);
    }
  }
}

/// Diffuses nutrients using a Moore neighborhood.
///
/// @param [in] diffusion_rate Diffusion rate for nutrients.
/// @param [in] neighborhood Nutrient values of neighboring cells.
/// @return Updated nutrients value after diffusion.
double
apply_site_diffusion(double diffusion_rate, data::NutrientNeighborhood &neighborhood) {
  double site = neighborhood.site;
  data::EightNutrients neighbors = neighborhood.neighbors;

  double neighbor_sum = 0.0;
  for (int i = 0; i < 8; i++) {
    neighbor_sum += neighbors[i];
  }

  return (1 - 8 * diffusion_rate) * site + diffusion_rate * neighbor_sum;
}
}   // namespace transformers
