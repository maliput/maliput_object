// Copyright 2022 Toyota Research Institute
#pragma once

#include <maliput/math/roll_pitch_yaw.h>
#include <maliput/math/vector.h>

#include "maliput_object/api/bounding_region.h"

namespace maliput {
namespace object {

/// Implements BoundingRegion abstract class for box-shaped bounding regions.
class BoundingBox : public api::BoundingRegion<maliput::math::Vector3> {
 public:
  MALIPUT_OBJECT_DEFAULT_COPY_AND_MOVE_AND_ASSIGN(BoundingBox)

  /// Constructs a BoundingBox object.
  /// The box is defined by a center, dimensions(length, width and height) and orientation.
  /// @param center Center of the bounding box in the Inertial-frame.
  /// @param box_size The size of the bounding box on XYZ (length/width,height)
  /// @param orientation Orientation of the box in the Inertial-frame.
  BoundingBox(const maliput::math::Vector3& center, const maliput::math::Vector3& box_size,
              const maliput::math::RollPitchYaw& orientation, double tolerance);

  BoundingBox() = delete;
  ~BoundingBox() = default;

  /// @returns The vertices of the bounding box in the Inertial-frame.
  std::vector<maliput::math::Vector3> get_vertices() const;

  /// @returns The orientation of the box in the Inertial-frame.
  const maliput::math::RollPitchYaw& get_orientation() const;

  /// @returns The size of the box in length, width and height.
  const maliput::math::Vector3& box_size() const;

 private:
  const maliput::math::Vector3& do_center() const override;
  bool do_contains(const maliput::math::Vector3& position) const override;
  bool do_contains(const api::BoundingRegion<maliput::math::Vector3>& other) const override;
  bool do_intersects(const api::BoundingRegion<maliput::math::Vector3>& other) const override;

  maliput::math::Vector3 center_;
  maliput::math::Vector3 box_size_;
  maliput::math::RollPitchYaw orientation_;
  double tolerance_{};
};

}  // namespace object
}  // namespace maliput
