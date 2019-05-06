#ifndef BIOFILM_SIMULATION_INITIALIZATION_SYSTEM_HPP
#define BIOFILM_SIMULATION_INITIALIZATION_SYSTEM_HPP

#include <random>

#include <entities.hpp>
#include <utilities.hpp>

namespace initializers {
struct BacteriumRandomInitializer {
  using UniformRNG = std::uniform_real_distribution<double>;

  std::mt19937 &random_engine_;
  UniformRNG uniform_rng_;
  double probability_;

  BacteriumRandomInitializer(std::mt19937 &random_engine, double probability);
  int operator()();
};

struct EntityInitializer {
private:
  Entities &entities_;

public:
  double max_nutrient_;
  BacteriumRandomInitializer kbacterium_initializer_;

  EntityInitializer(
      std::mt19937 &random_engine, double probability, double max_nutrient,
      Entities &entities);
  Entities::EntityID operator()(data::EntityInitializationParameters parameters);
};

Entities initialize_entities(
    const data::InitializationParameters &parameters, utilities::IndexConverter &indexer,
    std::mt19937 &random_engine);

}   // namespace initializers

#endif   // BIOFILM_SIMULATION_INITIALIZATION_SYSTEM_HPP
