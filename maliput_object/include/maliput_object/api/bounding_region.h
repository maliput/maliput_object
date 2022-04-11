// Copyright 2022 Toyota Research Institute
#pragma once

#include "maliput_object/common/macros.h"

namespace maliput {
namespace object {
namespace api {

/// Abstract API for bounding description.
/// @tparam Coordinate Coordinate in a given coordinate system.
template <typename Coordinate>
class BoundingRegion {
 public:
  MALIPUT_OBJECT_DEFAULT_COPY_AND_MOVE_AND_ASSIGN(BoundingRegion)

  virtual ~BoundingRegion() = default;

  /// Obtains the center coordinate of the bounding region in the Inertial-frame.
  /// @returns The center coordinate.
  const Coordinate& center() const { return do_center(); }

  /// Determines whether a @p position in the Inertial-frame is contained in this bounding region.
  /// @param position Inertial-frame's coordinate.
  /// @returns True when @p position is contained in this bounding region.
  bool Contains(const Coordinate& position) const { return do_contains(position); }

  /// Determines whether @p other BoundingRegion instance is contained in this bounding region.
  /// @param other Another BoundingRegion.
  /// @returns True when @p other is contained in this bounding region.
  bool Contains(const BoundingRegion<Coordinate>& other) const { return do_contains(other); }

  /// Determines whether @p other BoundingRegion instance intersects this bounding region.
  /// @param other Another BoundingRegion.
  /// @returns True when @p other intersects this bounding region.
  bool Intersects(const BoundingRegion<Coordinate>& other) const { return do_intersects(other); }

 protected:
  BoundingRegion() = default;

 private:
  virtual const Coordinate& do_center() const = 0;
  virtual bool do_contains(const Coordinate& position) const = 0;
  virtual bool do_contains(const BoundingRegion<Coordinate>& other) const = 0;
  virtual bool do_intersects(const BoundingRegion<Coordinate>& other) const = 0;
};

}  // namespace api
}  // namespace object
}  // namespace maliput
