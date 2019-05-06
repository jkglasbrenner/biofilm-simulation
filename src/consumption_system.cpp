#include <consumption_system.hpp>

#include <algorithm>

#include <entities.hpp>

namespace transformers {
void apply_consumption_transformation(Entities &entities, double consumed) {
  int num_entities = entities.names_.size();
  uint32_t transform_flag = Entities::kFlagBacteria | Entities::kFlagNutrients;

  for (Entities::EntityID id = 0; id < num_entities; id++) {
    uint32_t flag_check = transform_flag & entities.flags_[id];
    if (flag_check == transform_flag) {
      if (entities.bacteria_[id] == Entities::kStateBacterium) {
        entities.nutrients_[id] = std::max(0.0, entities.nutrients_[id] - consumed);
      }
    }
  }
}
}   // namespace transformers
