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
  /// Holds the possible bounding overlapping types.
  enum class OverlappingType {
    kDisjointed = 0,  ///< No overlapping between bounding regions
    kIntersected,     ///< Bounding regions intersects.
    kContained        ///< Entire bounding region is contained within another.
  };

  MALIPUT_OBJECT_DEFAULT_COPY_AND_MOVE_AND_ASSIGN(BoundingRegion)

  virtual ~BoundingRegion() = default;

  /// Obtains the center coordinate of the bounding region in the Inertial-frame.
  /// The center is defined as the centroid (center of area) of the bounding region,
  /// which comes from considering the surface as having constant density.
  /// @returns The center coordinate.
  const Coordinate& center() const { return do_center(); }

  /// Determines whether a @p position in the Inertial-frame is contained in this bounding region.
  /// @param position Inertial-frame's coordinate.
  /// @returns True when @p position is contained in this bounding region.
  bool Contains(const Coordinate& position) const { return do_contains(position); }

  /// Determines the overlapping type with @p other BoundingRegion instance.
  /// @param other Another BoundingRegion.
  /// @returns The overlapping type.
  OverlappingType Overlaps(const BoundingRegion<Coordinate>& other) const { do_overlaps(other); }

 protected:
  BoundingRegion() = default;

 private:
  virtual const Coordinate& do_center() const = 0;
  virtual bool do_contains(const Coordinate& position) const = 0;
  virtual OverlappingType do_overlaps(const BoundingRegion<Coordinate>& other) const = 0;
};

}  // namespace api
}  // namespace object
}  // namespace maliput
