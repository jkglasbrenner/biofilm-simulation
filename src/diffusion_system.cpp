#include <diffusion_system.hpp>

#include <data.hpp>
#include <entities.hpp>
#include <neighbors.hpp>

namespace transformers {
void apply_diffusion_transformation(
    Entities &entities, double diffusion_rate,
    const data::GetNeighborsParameters &get_neighbors_parameters) {
  int num_entities = entities.names_.size();
  uint32_t transform_flag = Entities::kFlagNutrients | Entities::kFlagNutrientsCopy;

  for (Entities::EntityID id = 0; id < num_entities; id++) {
    uint32_t flag_check = transform_flag & entities.flags_[id];

    if (flag_check == transform_flag) {
      data::EightNeighborIDs neighbor_ids =
          neighbors::get_nutrient_neighbor_ids(id, entities, get_neighbors_parameters);

      data::NutrientNeighborhood nutrient_neighborhood{};
      nutrient_neighborhood.site = entities.nutrients_copy_[id];
      nutrient_neighborhood.neighbors =
          neighbors::get_nutrient_neighbor_values(entities, neighbor_ids);

      entities.nutrients_[id] =
          apply_site_diffusion(diffusion_rate, nutrient_neighborhood);
    }
  }
}

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
