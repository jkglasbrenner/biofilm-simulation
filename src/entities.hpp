#ifndef BIOFILM_SIMULATION_ENTITIES_HPP
#define BIOFILM_SIMULATION_ENTITIES_HPP

#include <string>
#include <vector>

#include <components.hpp>

/// Entities are collections of components that represent the grid cells and
/// states in the simulation.
///
/// The Entities struct follows the "structure of arrays" pattern to better
/// ensure sequential memory access during the simulation. Instead of creating
/// the various grids separately, they are defined in parallel and we use a
/// series of flags (defined as a bitfield) to toggle components on and off.
/// This separation allows us to define padded cells as having active
/// bacteria_copy and nutrients_copy components but inactive bacteria and
/// nutrients components. By checking for which components are flagged as
/// active, we can quickly discern if an entity represents a padded cell or a
/// regular cell.
struct Entities {
  typedef int EntityID;

  /// Bitfield for toggling which components are active for an entity.
  enum : uint32_t {
    kFlagCellIndices = 1u << 0u,     ///< Toggle indices component
    kFlagCellLocations = 1u << 1u,   ///< Toggle locations component
    kFlagBacteria = 1u << 2u,        ///< Toggle bacteria component
    kFlagNutrients = 1u << 3u,       ///< Toggle nutrients component
    kFlagBacteriaCopy = 1u << 4u,    ///< Toggle bacteria_copy component
    kFlagNutrientsCopy = 1u << 5u,   ///< Toggle nutrients_copy component.
  };

  /// Available states in bacteria component.
  enum : int {
    kStateEmpty = 0,       ///< Empty grid cell.
    kStateBacterium = 1,   ///< Live bacterium in cell.
    kStateDead = 2,        ///< Dead bacterium in cell.
    kStateBorder = 3,      ///< Cell is in the padded region.
  };

  component::Names names_;                ///< A plain text name for help in
                                          ///< identifying an entity.
                                          ///<
  component::CellIndices indices_;        ///< The (row, column) for an entity.
                                          ///<
  component::CellLocations locations_;    ///< The location label for an entity.
                                          ///<
  component::Bacteria bacteria_;          ///< The bacteria grid state of an
                                          ///< entity.
                                          ///<
  component::Nutrients nutrients_;        ///< The nutrients grid state of an
                                          ///< entity.
                                          ///<
  component::Bacteria bacteria_copy_;     ///< The bacteria_copy grid state of an
                                          ///< entity.
                                          ///<
  component::Nutrients nutrients_copy_;   ///< The nutrients_copy grid state of an
                                          ///< entity.
                                          ///<
  component::Flags flags_;                ///< Bitfield for identifying which
                                          ///< components are active and which are
                                          ///< not for any given entity.

  /// Reserve memory for a set of entities.
  ///
  /// @param [in] n The number of entities you expect to create.
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

  /// Create a new empty entity.
  ///
  /// @param [in] name A plain text name for help in identifying the created
  ///   entity.
  /// @return The unique integer ID for the entity.
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
