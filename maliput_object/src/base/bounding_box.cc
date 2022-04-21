// Copyright 2022 Toyota Research Institute
#include "maliput_object/base/bounding_box.h"

namespace maliput {
namespace object {

BoundingBox::BoundingBox(const maliput::math::Vector3& position, const maliput::math::Vector3& box_size,
                         const maliput::math::RollPitchYaw& orientation, double tolerance)
    : position_(position),
      box_size_(box_size),
      orientation_(orientation),
      tolerance_(tolerance),
      xyz_2_(box_size.x() / 2., box_size.y() / 2., box_size.z() / 2.) {
  MALIPUT_THROW_UNLESS(tolerance >= 0.);
}

const maliput::math::Vector3& BoundingBox::do_position() const { return position_; }

std::vector<maliput::math::Vector3> BoundingBox::get_vertices() const {
  const std::vector<maliput::math::Vector3> vertices_box_frame{
      {xyz_2_.x(), xyz_2_.y(), xyz_2_.z()},   {-xyz_2_.x(), xyz_2_.y(), xyz_2_.z()},
      {xyz_2_.x(), -xyz_2_.y(), xyz_2_.z()},  {xyz_2_.x(), xyz_2_.y(), -xyz_2_.z()},
      {-xyz_2_.x(), -xyz_2_.y(), xyz_2_.z()}, {xyz_2_.x(), -xyz_2_.y(), -xyz_2_.z()},
      {-xyz_2_.x(), xyz_2_.y(), -xyz_2_.z()}, {-xyz_2_.x(), -xyz_2_.y(), -xyz_2_.z()},
  };
  std::vector<maliput::math::Vector3> vertices;
  for (const auto& vertex : vertices_box_frame) {
    vertices.push_back(orientation_.ToMatrix().inverse() * vertex + position_);
  }
  return vertices;
}

const maliput::math::RollPitchYaw& BoundingBox::get_orientation() const { return orientation_; }

bool BoundingBox::DoContains(const maliput::math::Vector3& position) const {
  const maliput::math::Vector3 box_frame_position = orientation_.ToMatrix() * (position - position_);
  return box_frame_position.x() <= xyz_2_.x() + tolerance_ && box_frame_position.x() >= -xyz_2_.x() - tolerance_ &&
         box_frame_position.y() <= xyz_2_.y() + tolerance_ && box_frame_position.y() >= -xyz_2_.y() - tolerance_ &&
         box_frame_position.z() <= xyz_2_.z() + tolerance_ && box_frame_position.z() >= -xyz_2_.z() - tolerance_;
}

BoundingBox::OverlappingType BoundingBox::DoOverlaps(const api::BoundingRegion<maliput::math::Vector3>& other) const {
  auto other_box = dynamic_cast<const BoundingBox*>(&other);
  MALIPUT_VALIDATE(other_box != nullptr, "BoundingRegion's implementations supported: BoundingBox.");
  if (IsBoxContained(*other_box)) {
    return BoundingBox::OverlappingType::kContained;
  }
  if (IsBoxIntersected(*other_box)) {
    return BoundingBox::OverlappingType::kIntersected;
  }
  return BoundingBox::OverlappingType::kDisjointed;
}

bool BoundingBox::IsBoxContained(const BoundingBox& other) const {
  const auto vertices = other.get_vertices();
  return std::all_of(vertices.begin(), vertices.end(), [this](const auto& vertex) { return this->DoContains(vertex); });
}

bool BoundingBox::IsBoxIntersected(const BoundingBox& other) const {
  // TODO: Implement method. Check Oriented Bounding Box Intersection detection using SAT.
  return false;
}

}  // namespace object
}  // namespace maliput
