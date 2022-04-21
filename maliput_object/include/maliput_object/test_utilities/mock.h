// Copyright 2022 Toyota Research Institute
#include <gtest/gtest.h>
#include <maliput/math/vector.h>

#include "maliput_object/api/bounding_region.h"

namespace maliput {
namespace object {
namespace test_utilities {

// Mocks BoundaryRegion class hardcoding the api's return values.
class MockBoundingRegion : public api::BoundingRegion<maliput::math::Vector3> {
 public:
  MockBoundingRegion() = default;
  MockBoundingRegion(const maliput::math::Vector3& position, bool contains,
                     const api::BoundingRegion<maliput::math::Vector3>::OverlappingType& overlapping_type)
      : position_(position), contains_(contains), overlapping_type_(overlapping_type) {}
  const maliput::math::Vector3& do_position() const override { return position_; }
  bool DoContains(const maliput::math::Vector3& position) const override { return contains_; }
  OverlappingType DoOverlaps(const BoundingRegion<maliput::math::Vector3>& other) const override {
    return overlapping_type_;
  }

  void SetPosition(const maliput::math::Vector3& position) { position_ = position; }
  void SetContains(bool contains) { contains_ = contains; }
  void SetOverlappingType(const OverlappingType& overlapping_type) { overlapping_type_ = overlapping_type; }

 private:
  maliput::math::Vector3 position_{0., 0., 0.};
  bool contains_{false};
  OverlappingType overlapping_type_{OverlappingType::kDisjointed};
};

}  // namespace test_utilities
}  // namespace object
}  // namespace maliput
