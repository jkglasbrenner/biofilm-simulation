#include <initializers.hpp>

#include <random>

namespace initializers {
Entities initialize_entities(
    const data::InitializationParameters &parameters, utilities::IndexConverter &indexer,
    std::mt19937 &random_engine) {

  int number_entities = utilities::count_elements(parameters.dimensions);
  Entities entities = Entities();
  entities.reserve(number_entities);

  EntityInitializer entity_initializer = EntityInitializer(
      random_engine, parameters.probability_bacterium, parameters.max_nutrient, entities);

  data::CellLocationParameters cell_location_parameters{};
  cell_location_parameters.left_padding_boundary = parameters.dimensions.size_padding;
  cell_location_parameters.right_padding_boundary =
      parameters.dimensions.number_columns + parameters.dimensions.size_padding - 1;
  cell_location_parameters.top_padding_boundary = parameters.dimensions.size_padding;
  cell_location_parameters.bottom_padding_boundary =
      parameters.dimensions.number_rows + parameters.dimensions.size_padding - 1;

  for (int i = 0; i < number_entities; i++) {
    cell_location_parameters.index = indexer(i);
    int cell_location = utilities::get_cell_location(cell_location_parameters);
    Entities::EntityID entity_id =
        entity_initializer(data::EntityInitializationParameters{
            cell_location_parameters.index, cell_location});
  }

  return entities;
}

EntityInitializer::EntityInitializer(
    std::mt19937 &random_engine, double probability, double max_nutrient,
    Entities &entities)
    : entities_(entities), max_nutrient_(max_nutrient),
      kbacterium_initializer_(BacteriumRandomInitializer(random_engine, probability)){};

Entities::EntityID EntityInitializer::
operator()(data::EntityInitializationParameters parameters) {
  int id = -1;

  switch (parameters.cell_location) {
    case utilities::kCellLocationLeftPad:
      id = entities_.add_entity("LeftPad");
      entities_.bacteria_copy_[id] = Entities::kStateBorder;
      entities_.indices_[id].row = parameters.index.row;
      entities_.indices_[id].column = parameters.index.column;
      entities_.locations_[id] = utilities::kCellLocationLeftPad;
      entities_.flags_[id] |= Entities::kFlagBacteriaCopy;
      entities_.flags_[id] |= Entities::kFlagNutrientsCopy;
      entities_.flags_[id] |= Entities::kFlagCellIndices;
      entities_.flags_[id] |= Entities::kFlagCellLocations;
      break;

    case utilities::kCellLocationRightPad:
      id = entities_.add_entity("RightPad");
      entities_.bacteria_copy_[id] = Entities::kStateBorder;
      entities_.nutrients_copy_[id] = max_nutrient_;
      entities_.indices_[id].row = parameters.index.row;
      entities_.indices_[id].column = parameters.index.column;
      entities_.locations_[id] = utilities::kCellLocationRightPad;
      entities_.flags_[id] |= Entities::kFlagBacteriaCopy;
      entities_.flags_[id] |= Entities::kFlagNutrientsCopy;
      entities_.flags_[id] |= Entities::kFlagCellIndices;
      entities_.flags_[id] |= Entities::kFlagCellLocations;
      break;

    case utilities::kCellLocationTopPad:
      id = entities_.add_entity("TopPad");
      entities_.indices_[id].row = parameters.index.row;
      entities_.indices_[id].column = parameters.index.column;
      entities_.locations_[id] = utilities::kCellLocationTopPad;
      entities_.flags_[id] |= Entities::kFlagBacteriaCopy;
      entities_.flags_[id] |= Entities::kFlagNutrientsCopy;
      entities_.flags_[id] |= Entities::kFlagCellIndices;
      entities_.flags_[id] |= Entities::kFlagCellLocations;
      break;

    case utilities::kCellLocationBottomPad:
      id = entities_.add_entity("BottomPad");
      entities_.indices_[id].row = parameters.index.row;
      entities_.indices_[id].column = parameters.index.column;
      entities_.locations_[id] = utilities::kCellLocationBottomPad;
      entities_.flags_[id] |= Entities::kFlagBacteriaCopy;
      entities_.flags_[id] |= Entities::kFlagNutrientsCopy;
      entities_.flags_[id] |= Entities::kFlagCellIndices;
      entities_.flags_[id] |= Entities::kFlagCellLocations;
      break;

    case utilities::kCellLocationFirstColumn:
      id = entities_.add_entity("Cell");
      entities_.bacteria_[id] = kbacterium_initializer_();
      entities_.nutrients_[id] = max_nutrient_;
      entities_.indices_[id].row = parameters.index.row;
      entities_.indices_[id].column = parameters.index.column;
      entities_.locations_[id] = utilities::kCellLocationFirstColumn;
      entities_.flags_[id] |= Entities::kFlagBacteria;
      entities_.flags_[id] |= Entities::kFlagNutrients;
      entities_.flags_[id] |= Entities::kFlagBacteriaCopy;
      entities_.flags_[id] |= Entities::kFlagNutrientsCopy;
      entities_.flags_[id] |= Entities::kFlagCellIndices;
      entities_.flags_[id] |= Entities::kFlagCellLocations;
      break;

    case utilities::kCellLocationLastColumn:
      id = entities_.add_entity("Cell");
      entities_.nutrients_[id] = max_nutrient_;
      entities_.indices_[id].row = parameters.index.row;
      entities_.indices_[id].column = parameters.index.column;
      entities_.locations_[id] = utilities::kCellLocationLastColumn;
      entities_.flags_[id] |= Entities::kFlagBacteria;
      entities_.flags_[id] |= Entities::kFlagNutrients;
      entities_.flags_[id] |= Entities::kFlagBacteriaCopy;
      entities_.flags_[id] |= Entities::kFlagNutrientsCopy;
      entities_.flags_[id] |= Entities::kFlagCellIndices;
      entities_.flags_[id] |= Entities::kFlagCellLocations;
      break;

    case utilities::kCellLocationOther:
      id = entities_.add_entity("cell");
      entities_.nutrients_[id] = max_nutrient_;
      entities_.indices_[id].row = parameters.index.row;
      entities_.indices_[id].column = parameters.index.column;
      entities_.locations_[id] = utilities::kCellLocationOther;
      entities_.flags_[id] |= Entities::kFlagBacteria;
      entities_.flags_[id] |= Entities::kFlagNutrients;
      entities_.flags_[id] |= Entities::kFlagBacteriaCopy;
      entities_.flags_[id] |= Entities::kFlagNutrientsCopy;
      entities_.flags_[id] |= Entities::kFlagCellIndices;
      entities_.flags_[id] |= Entities::kFlagCellLocations;
      break;
  }

  return id;
}

BacteriumRandomInitializer::BacteriumRandomInitializer(
    std::mt19937 &random_engine, double probability)
    : random_engine_(random_engine), uniform_rng_(UniformRNG(0.0, 1.0)),
      probability_(probability){};

int BacteriumRandomInitializer::operator()() {
  double random_number = uniform_rng_(random_engine_);

  if (random_number < probability_) {
    return Entities::kStateBacterium;
  } else {
    return Entities::kStateEmpty;
  }
}

}   // namespace initializers
