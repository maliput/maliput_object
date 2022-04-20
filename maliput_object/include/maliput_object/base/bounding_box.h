// Copyright 2022 Toyota Research Institute
#pragma once

#include <vector>

#include <maliput/common/maliput_copyable.h>
#include <maliput/math/roll_pitch_yaw.h>
#include <maliput/math/vector.h>

#include "maliput_object/api/bounding_region.h"

namespace maliput {
namespace object {

/// Implements BoundingRegion abstract class for non-axis-aligned-box-shaped bounding regions.
class BoundingBox : public api::BoundingRegion<maliput::math::Vector3> {
 public:
  MALIPUT_DEFAULT_COPY_AND_MOVE_AND_ASSIGN(BoundingBox)

  /// Constructs a BoundingBox object.
  /// The box is defined by a position, dimensions(length, width and height) and orientation.
  /// @param position Position of the bounding box in the Inertial-frame. The position matches with the centroid of the
  /// box.
  /// @param box_size The size of the bounding box on XYZ (length/width,height)
  /// @param orientation Orientation of the box in the Inertial-frame.
  BoundingBox(const maliput::math::Vector3& position, const maliput::math::Vector3& box_size,
              const maliput::math::RollPitchYaw& orientation, double tolerance);

  ~BoundingBox() = default;

  /// @returns The vertices of the bounding box in the Inertial-frame.
  std::vector<maliput::math::Vector3> get_vertices() const;

  /// @returns The orientation of the box in the Inertial-frame.
  const maliput::math::RollPitchYaw& get_orientation() const;

  /// @returns The size of the box in length, width and height.
  const maliput::math::Vector3& box_size() const;

  /// @returns True when this region contains @p other .
  bool IsBoxContained(const BoundingBox& other) const;

  /// @returns True when this region intersects @p other .
  bool IsBoxIntersected(const BoundingBox& other) const;

 private:
  /// Implements api::BoundingRegion::do_position() method.
  /// @returns Position of the box.
  const maliput::math::Vector3& do_position() const override;

  /// Implements api::BoundingRegion::DoContains() method.
  /// @param position Inertial-frame's coordinate.
  /// @returns True when @p position is contained in this bounding region.
  bool DoContains(const maliput::math::Vector3& position) const override;

  /// Implements api::BoundingRegion::DoOverlaps() method.
  /// Valid @p other 's implementations:
  ///  - BoundingBox
  /// @param other Another bounding region.
  /// @returns The overlapping type.
  OverlappingType DoOverlaps(const api::BoundingRegion<maliput::math::Vector3>& other) const override;

  maliput::math::Vector3 position_;
  maliput::math::Vector3 box_size_;
  maliput::math::RollPitchYaw orientation_;
  double tolerance_{};
};

}  // namespace object
}  // namespace maliput
