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

  /// Gets all the Objects in the book.
  /// @returns A unordered map, from which key and value are Object::Id and pointer to Object respectively.
  std::unordered_map<typename Object<Coordinate>::Id, Object<Coordinate>*> objects() const { return do_objects(); }

  /// Finds Object by Id.
  /// @param object_id An Object::Id.
  /// @returns A valid Object's pointer if found, nullptr otherwise.
  Object<Coordinate>* FindBy(const typename Object<Coordinate>::Id& object_id) const { return DoFindBy(object_id); }

  /// Finds the Objects that make @p predicate true.
  /// @param predicate Unary predicate for evaluating an object.
  std::vector<Object<Coordinate>*> FindBy(std::function<bool(const Object<Coordinate>*)> predicate) const {
    return DoFindBy(predicate);
  }

  /// Finds the Objects that intersect with a @p region .
  /// @param region BoundaryRegion used for finding intersected Objects.
  /// @param overlapping_type Optional argument for adding the overlapping type as a constraint.
  ///                         By default it is std::nullopt.
  /// @returns The Objects intersecting @p region with the given @p overlapping_type .
  std::vector<Object<Coordinate>*> FindOverlappingIn(
      const BoundingRegion<Coordinate>& region,
      const std::optional<typename BoundingRegion<Coordinate>::OverlappingType>& overlapping_type =
          std::nullopt) const {
    return DoFindOverlappingIn(region, overlapping_type);
  }

 protected:
  ObjectBook() = default;

 private:
  virtual std::unordered_map<typename Object<Coordinate>::Id, Object<Coordinate>*> do_objects() const = 0;
  virtual Object<Coordinate>* DoFindBy(const typename Object<Coordinate>::Id& object_id) const = 0;
  virtual std::vector<Object<Coordinate>*> DoFindBy(std::function<bool(const Object<Coordinate>*)> predicate) const = 0;
  virtual std::vector<Object<Coordinate>*> DoFindOverlappingIn(
      const BoundingRegion<Coordinate>& region,
      const std::optional<typename BoundingRegion<Coordinate>::OverlappingType>& overlapping_type) const = 0;
};

}  // namespace api
}  // namespace object
}  // namespace maliput
