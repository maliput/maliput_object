// Copyright 2022 Toyota Research Institute
#pragma once

#include <maliput/common/maliput_copyable.h>

namespace maliput {
namespace object {
namespace api {

/// Abstract API for bounding description.
/// @tparam Coordinate Coordinate in a given coordinate system.
template <typename Coordinate>
class BoundingRegion {
 public:
  /// Holds the possible bounding overlapping types.
  enum class OverlappingType {
    kDisjointed = 0,  ///< No overlapping between bounding regions
    kIntersected,     ///< Bounding regions intersects.
    kContained        ///< Entire bounding region is contained within another.
  };

  MALIPUT_DEFAULT_COPY_AND_MOVE_AND_ASSIGN(BoundingRegion)

  virtual ~BoundingRegion() = default;

  /// Obtains the bounding region's position in the Inertial-frame.
  /// The position is expected to match the centroid of the bounding region.
  /// @returns The position coordinate.
  const Coordinate& position() const { return do_position(); }

  /// Determines whether a @p position in the Inertial-frame is contained in this bounding region.
  /// @param position Inertial-frame's coordinate.
  /// @returns True when @p position is contained in this bounding region.
  bool Contains(const Coordinate& position) const { return DoContains(position); }

  /// Determines the overlapping type with @p other BoundingRegion instance.
  /// - OverlappingType::kDisjointed is returned when there is no overlapping with @p other .
  /// - OverlappingType::kIntersected is returned when @p other intersects with this region.
  /// - OverlappingType::kContained is returned when @p other is contained within this region.
  /// @param other Another BoundingRegion.
  /// @returns The overlapping type.
  OverlappingType Overlaps(const BoundingRegion<Coordinate>& other) const { return DoOverlaps(other); }

 protected:
  BoundingRegion() = default;

 private:
  virtual const Coordinate& do_position() const = 0;
  virtual bool DoContains(const Coordinate& position) const = 0;
  virtual OverlappingType DoOverlaps(const BoundingRegion<Coordinate>& other) const = 0;
};

}  // namespace api
}  // namespace object
}  // namespace maliput
