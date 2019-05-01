#ifndef BIOFILM_SIMULATION_ENTITIES_HPP
#define BIOFILM_SIMULATION_ENTITIES_HPP

#include <string>
#include <vector>

#include <components.hpp>

struct Entities {
  typedef int EntityID;

  enum : uint32_t {
    kFlagCellIndices = 1u << 0u,
    kFlagCellLocations = 1u << 1u,
    kFlagBacteria = 1u << 2u,
    kFlagNutrients = 1u << 3u,
    kFlagBacteriaCopy = 1u << 4u,
    kFlagNutrientsCopy = 1u << 5u,
  };

  enum : int {
    kStateEmpty = 0,
    kStateBacterium = 1,
    kStateDead = 2,
    kStateBorder = 3,
  };

  component::Names names_;
  component::CellIndices indices_;
  component::CellLocations locations_;
  component::Bacteria bacteria_;
  component::Nutrients nutrients_;
  component::Bacteria bacteria_copy_;
  component::Nutrients nutrients_copy_;
  component::Flags flags_;

  void reserve(int n) {
    names_.reserve(n);
    indices_.reserve(n);
    locations_.reserve(n);
    bacteria_.reserve(n);
    nutrients_.reserve(n);
    bacteria_copy_.reserve(n);
    nutrients_copy_.reserve(n);
    flags_.reserve(n);
  }

  EntityID add_entity(std::string &&name) {
    EntityID id = names_.size();

    names_.emplace_back(name);
    indices_.push_back(data::Index());
    locations_.push_back(-1);
    bacteria_.push_back(kStateEmpty);
    nutrients_.push_back(0.0);
    bacteria_copy_.push_back(kStateEmpty);
    nutrients_copy_.push_back(0.0);
    flags_.push_back(0u);

    return id;
  }
};
#endif   // BIOFILM_SIMULATION_ENTITIES_HPP
