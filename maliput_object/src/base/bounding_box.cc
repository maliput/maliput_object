// Copyright 2022 Toyota Research Institute
#include "maliput_object/base/bounding_box.h"

namespace maliput {
namespace object {

BoundingBox::BoundingBox(const maliput::math::Vector3& center, const maliput::math::Vector3& box_size,
                         const maliput::math::RollPitchYaw& orientation, double tolerance)
    : center_(center), box_size_(box_size), orientation_(orientation), tolerance_(tolerance) {
  MALIPUT_OBJECT_THROW_UNLESS(tolerance >= 0.);
}

const maliput::math::Vector3& BoundingBox::do_center() const { return center_; }

std::vector<maliput::math::Vector3> BoundingBox::get_vertices() const {
  const std::vector<maliput::math::Vector3> vertices_box_frame{
      {box_size_.x() / 2., box_size_.y() / 2, box_size_.z() / 2},
      {-box_size_.x() / 2., box_size_.y() / 2, box_size_.z() / 2},
      {box_size_.x() / 2., -box_size_.y() / 2, box_size_.z() / 2},
      {box_size_.x() / 2., box_size_.y() / 2, -box_size_.z() / 2},
      {-box_size_.x() / 2., -box_size_.y() / 2, box_size_.z() / 2},
      {box_size_.x() / 2., -box_size_.y() / 2, -box_size_.z() / 2},
      {-box_size_.x() / 2., box_size_.y() / 2, -box_size_.z() / 2},
      {-box_size_.x() / 2., -box_size_.y() / 2, -box_size_.z() / 2},
  };
  std::vector<maliput::math::Vector3> vertices;
  for (const auto& vertex : vertices_box_frame) {
    vertices.push_back(orientation_.ToMatrix().inverse() * vertex + center_);
  }
  return vertices;
}

const maliput::math::RollPitchYaw& BoundingBox::get_orientation() const { return orientation_; }

bool BoundingBox::do_contains(const maliput::math::Vector3& position) const {
  const maliput::math::Vector3 box_frame_position = orientation_.ToMatrix() * (position - center_);
  return box_frame_position.x() <= box_size_.x() / 2. + tolerance_ &&
         box_frame_position.x() >= -box_size_.x() / 2. - tolerance_ &&
         box_frame_position.y() <= box_size_.y() / 2. + tolerance_ &&
         box_frame_position.y() >= -box_size_.y() / 2. - tolerance_ &&
         box_frame_position.z() <= box_size_.z() / 2. + tolerance_ &&
         box_frame_position.z() >= -box_size_.z() / 2. - tolerance_;
}

bool BoundingBox::do_contains(const api::BoundingRegion<maliput::math::Vector3>& other) const {
  auto other_box = dynamic_cast<const BoundingBox*>(&other);
  MALIPUT_THROW_UNLESS(other_box != nullptr);
  const auto vertices = other_box->get_vertices();
  for (const auto vertex : vertices) {
    if (!do_contains(vertex)) {
      return false;
    }
  }
  return true;
}

bool BoundingBox::do_intersects(const api::BoundingRegion<maliput::math::Vector3>&) const {
  // TODO: Implement method. Check Oriented Bounding Box Intersection detection using SAT.
  MALIPUT_OBJECT_THROW_MESSAGE("Unimplemented method");
}

}  // namespace object
}  // namespace maliput
