#include <copy_system.hpp>

#include <entities.hpp>
#include <utilities.hpp>

namespace transformers {
/// Refresh values of padded (ghost) cells that wrap around grid.
///
/// Padded (ghost) cells are used to implement boundary conditions
/// and keep track of component states from the previous time step.
///
/// @param [in,out] entities Entities struct with components defining the
///   nutrients and bacteria grids.
/// @param [in] cells_per_row The number of columns there are per row in the
///   padded grid.
void refresh_padded_states(Entities &entities, int cells_per_row) {
  int num_entities = entities.names_.size();

  for (int id = 0; id < num_entities; id++) {
    if (entities.locations_[id] == utilities::kCellLocationTopPad) {
      int neighbor_id = id + cells_per_row;
      entities.bacteria_copy_[id] = entities.bacteria_[neighbor_id];
      entities.nutrients_copy_[id] = entities.nutrients_[neighbor_id];
    } else if (entities.locations_[id] == utilities::kCellLocationBottomPad) {
      int neighbor_id = id - cells_per_row;
      entities.bacteria_copy_[id] = entities.bacteria_[neighbor_id];
      entities.nutrients_copy_[id] = entities.nutrients_[neighbor_id];
    }
  }
}

/// Copy from bacteria component into bacteria_copy component.
///
/// Copying is used to keep track of bacteria component state from previous time
/// step.
///
/// @param [in,out] entities Entities struct with components defining the
///   nutrients and bacteria grids.
void copy_bacteria_states(Entities &entities) {
  int num_entities = entities.names_.size();
  uint32_t transform_flag = Entities::kFlagBacteria | Entities::kFlagBacteriaCopy;

  for (int id = 0; id < num_entities; id++) {
    uint32_t flag_check = transform_flag & entities.flags_[id];
    if (flag_check == transform_flag) {
      entities.bacteria_copy_[id] = entities.bacteria_[id];
    }
  }
}

/// Copy from nutrients component into nutrients_copy component.
///
/// Copying is used to keep track of nutients component state from previous time
/// step.
///
/// @param [in,out] entities Entities struct with components defining the
///   nutrients and bacteria grids.
void copy_nutrients_states(Entities &entities) {
  int num_entities = entities.names_.size();
  uint32_t transform_flag = Entities::kFlagNutrients | Entities::kFlagNutrientsCopy;

  for (int id = 0; id < num_entities; id++) {
    uint32_t flag_check = transform_flag & entities.flags_[id];
    if (flag_check == transform_flag) {
      entities.nutrients_copy_[id] = entities.nutrients_[id];
    }
  }
}
}   // namespace transformers
