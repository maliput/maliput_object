// Copyright 2022 Toyota Research Institute
#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>

#include <maliput/common/maliput_copyable.h>
#include <maliput/math/vector.h>

#include "maliput_object/api/bounding_region.h"
#include "maliput_object/api/object.h"

namespace maliput {
namespace object {
namespace api {

/// Book for Objects in a given @p Coordinate system.
template <typename Coordinate>
class ObjectBook {
 public:
  MALIPUT_NO_COPY_NO_MOVE_NO_ASSIGN(ObjectBook)
  virtual ~ObjectBook() = default;

  /// Finds Object by Id.
  /// @param object_id An Object::Id.
  /// @returns A valid Object's pointer if found, nullptr otherwise.
  Object* FindBy(const Object::Id& object_id) const { return DoFindBy(object_id); }

  /// Gets all the Objects in the book.
  /// @returns A unordered map, from which key and value are Object::Id and pointer to Object respectively.
  std::unordered_map<Object::Id, Object*> objects() const { return do_objects(); }

  /// Finds the Objects that make @p predicate to be true.
  /// @param predicate Unary precate to be used for evaluating an object characteristic.
  std::vector<Object*> FindBy(std::function<bool(const Object*)> predicate) const { return DoFindBy(predicate); }

  /// Finds the Objects that intersect with an spacial location delimited by @p region .
  /// @param region BoundaryRegion used for finding intersected Objects.
  /// @param overlapping_type Optional argument for adding the overlapping type as a constraint. By default it is
  /// std::nullopt.
  /// @returns The Objects intersecting @p region with the given @p overlapping_type .
  std::vector<Object*> FindOverlappingIn(
      const BoundingRegion<Coordinate>& region,
      std::optional<BoundingRegion<Coordinate>::OverlappingType> overlapping_type = std::nullopt) const {
    return DoFindOverlappingIn(region, overlapping_type);
  }

 private:
  std::unordered_map<Object::Id, Object*> do_objects() const = 0;
  Object* DoFindBy(const Object::Id& object_id) const = 0;
  std::vector<Object*> DoFindBy(std::function<bool(const Object*)> predicate) const = 0;
  std::vector<Object*> DoFindOverlappingIn(const BoundingBox& box) const = 0;
};

}  // namespace api
}  // namespace object
}  // namespace maliput
