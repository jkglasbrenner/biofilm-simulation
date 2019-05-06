#include <neighbors.hpp>

#include <random>

#include <data.hpp>
#include <entities.hpp>

namespace neighbors {
data::ThreeNeighborIDs get_first_column_bacterium_neighbors(
    Entities::EntityID id, const Entities &entities,
    const data::GetNeighborsParameters &parameters) {
  data::ThreeNeighborIDs neighborhood{};

  int top_neighbor;
  if (entities.indices_[id].row == parameters.first_unpadded_row) {
    top_neighbor = id + parameters.wraparound_shift;
  } else {
    top_neighbor = id - parameters.cells_per_row;
  }

  int bottom_neighbor;
  if (entities.indices_[id].row == parameters.last_unpadded_row) {
    bottom_neighbor = id - parameters.wraparound_shift;
  } else {
    bottom_neighbor = id + parameters.cells_per_row;
  }

  neighborhood[0] = id + 1;
  neighborhood[1] = top_neighbor;
  neighborhood[2] = bottom_neighbor;

  return neighborhood;
}

data::ThreeNeighborIDs get_last_column_bacterium_neighbors(
    Entities::EntityID id, const Entities &entities,
    const data::GetNeighborsParameters &parameters) {
  data::ThreeNeighborIDs neighborhood{};

  int top_neighbor;
  if (entities.indices_[id].row == parameters.first_unpadded_row) {
    top_neighbor = id + parameters.wraparound_shift;
  } else {
    top_neighbor = id - parameters.cells_per_row;
  }

  int bottom_neighbor;
  if (entities.indices_[id].row == parameters.last_unpadded_row) {
    bottom_neighbor = id - parameters.wraparound_shift;
  } else {
    bottom_neighbor = id + parameters.cells_per_row;
  }

  neighborhood[0] = id - 1;
  neighborhood[1] = top_neighbor;
  neighborhood[2] = bottom_neighbor;

  return neighborhood;
}

data::FourNeighborIDs get_bacterium_neighbors(
    Entities::EntityID id, const Entities &entities,
    const data::GetNeighborsParameters &parameters) {
  data::FourNeighborIDs neighborhood{};

  int top_neighbor;
  if (entities.indices_[id].row == parameters.first_unpadded_row) {
    top_neighbor = id + parameters.wraparound_shift;
  } else {
    top_neighbor = id - parameters.cells_per_row;
  }

  int bottom_neighbor;
  if (entities.indices_[id].row == parameters.last_unpadded_row) {
    bottom_neighbor = id - parameters.wraparound_shift;
  } else {
    bottom_neighbor = id + parameters.cells_per_row;
  }

  neighborhood[0] = id - 1;
  neighborhood[1] = id + 1;
  neighborhood[2] = top_neighbor;
  neighborhood[3] = bottom_neighbor;

  return neighborhood;
}

data::EightNeighborIDs get_nutrient_neighbor_ids(
    Entities::EntityID id, const Entities &entities,
    const data::GetNeighborsParameters &parameters) {
  data::EightNeighborIDs neighborhood{};

  int top_neighbor;
  if (entities.indices_[id].row == parameters.first_unpadded_row) {
    top_neighbor = id + parameters.wraparound_shift;
  } else {
    top_neighbor = id - parameters.cells_per_row;
  }

  int bottom_neighbor;
  if (entities.indices_[id].row == parameters.last_unpadded_row) {
    bottom_neighbor = id - parameters.wraparound_shift;
  } else {
    bottom_neighbor = id + parameters.cells_per_row;
  }

  neighborhood[0] = id - 1;
  neighborhood[1] = id + 1;
  neighborhood[2] = top_neighbor - 1;
  neighborhood[3] = top_neighbor;
  neighborhood[4] = top_neighbor + 1;
  neighborhood[5] = bottom_neighbor - 1;
  neighborhood[6] = bottom_neighbor;
  neighborhood[7] = bottom_neighbor + 1;

  return neighborhood;
}

data::EightNutrients get_nutrient_neighbor_values(
    const Entities &entities, const data::EightNeighborIDs &neighbor_ids) {
  data::EightNutrients nutrients{};

  for (int i = 0; i < 8; i++) {
    Entities::EntityID neighbor_id = neighbor_ids[i];
    nutrients[i] = entities.nutrients_copy_[neighbor_id];
  }

  return nutrients;
}

int sample_random_id(int num_neighbors, std::mt19937 &random_engine) {
  std::uniform_int_distribution<int> neighbor_rng(0, num_neighbors - 1);

  return neighbor_rng(random_engine);
}

bool check_for_empty_neighbors(
    Entities::EntityID id, const Entities &entities,
    const data::GetNeighborsParameters &parameters) {
  int cell_location = entities.locations_[id];

  bool empty_state_detected = false;
  if (cell_location == utilities::kCellLocationFirstColumn) {
    data::ThreeNeighborIDs neighborhood =
        get_first_column_bacterium_neighbors(id, entities, parameters);
    for (int i = 0; i < 3; i++) {
      int neighbor_id = neighborhood[i];
      int state = entities.bacteria_copy_[neighbor_id];
      if (state == Entities::kStateEmpty) {
        empty_state_detected = true;
      }
    }
  } else if (cell_location == utilities::kCellLocationLastColumn) {
    data::ThreeNeighborIDs neighborhood =
        get_last_column_bacterium_neighbors(id, entities, parameters);
    for (int i = 0; i < 3; i++) {
      int neighbor_id = neighborhood[i];
      int state = entities.bacteria_copy_[neighbor_id];
      if (state == Entities::kStateEmpty) {
        empty_state_detected = true;
      }
    }
  } else {
    data::FourNeighborIDs neighborhood =
        get_bacterium_neighbors(id, entities, parameters);
    for (int i = 0; i < 4; i++) {
      int neighbor_id = neighborhood[i];
      int state = entities.bacteria_copy_[neighbor_id];
      if (state == Entities::kStateEmpty) {
        empty_state_detected = true;
      }
    }
  }
  return empty_state_detected;
}

Entities::EntityID pick_bacterium_neighbor(
    Entities::EntityID id, const Entities &entities,
    const data::GetNeighborsParameters &parameters, std::mt19937 &random_engine) {
  int cell_location = entities.locations_[id];
  data::FourNeighborIDs empty_neighbor_ids{};
  int number_empty_neighbors = 0;

  if (cell_location == utilities::kCellLocationFirstColumn) {
    data::ThreeNeighborIDs neighborhood =
        get_first_column_bacterium_neighbors(id, entities, parameters);
    for (int i = 0; i < 3; i++) {
      int state = entities.bacteria_copy_[neighborhood[i]];
      if (state == Entities::kStateEmpty) {
        empty_neighbor_ids[number_empty_neighbors] = neighborhood[i];
        number_empty_neighbors++;
      }
    }
  } else if (cell_location == utilities::kCellLocationLastColumn) {
    data::ThreeNeighborIDs neighborhood =
        get_last_column_bacterium_neighbors(id, entities, parameters);
    for (int i = 0; i < 3; i++) {
      int state = entities.bacteria_copy_[neighborhood[i]];
      if (state == Entities::kStateEmpty) {
        empty_neighbor_ids[number_empty_neighbors] = neighborhood[i];
        number_empty_neighbors++;
      }
    }
  } else {
    data::FourNeighborIDs neighborhood =
        get_bacterium_neighbors(id, entities, parameters);
    for (int i = 0; i < 4; i++) {
      int state = entities.bacteria_copy_[neighborhood[i]];
      if (state == Entities::kStateEmpty) {
        empty_neighbor_ids[number_empty_neighbors] = neighborhood[i];
        number_empty_neighbors++;
      }
    }
  }
  int selected_neighbor = sample_random_id(number_empty_neighbors, random_engine);

  return empty_neighbor_ids[selected_neighbor];
}
}   // namespace neighbors
