// Copyright 2022 Toyota Research Institute
#include "maliput_object/api/bounding_region.h"

#include <memory>

#include <gtest/gtest.h>
#include <maliput/math/vector.h>

namespace maliput {
namespace object {
namespace test {
namespace {

using maliput::math::Vector3;

// Mocks BoundaryRegion class hardcoding the api's return values.
class MockBoundaryRegion : public api::BoundingRegion<Vector3> {
 public:
  const Vector3& do_position() const override { return position_; }
  bool DoContains(const Vector3& position) const override { return contains_; }

  OverlappingType DoOverlaps(const BoundingRegion<Vector3>& other) const override { return overlapping_type_; }

  void SetPosition(const Vector3& position) { position_ = position; }
  void SetContains(bool contains) { contains_ = contains; }
  void SetOverlappingType(const OverlappingType& overlapping_type) { overlapping_type_ = overlapping_type; }

 private:
  bool contains_{false};
  Vector3 position_{0., 0., 0.};
  OverlappingType overlapping_type_{OverlappingType::kDisjointed};
};

// Tests API interface.
class BoundingRegionTest : public ::testing::Test {
 public:
  void SetUp() override {
    auto mock_region = std::make_unique<MockBoundaryRegion>();
    mock_region->SetContains(kExpectedContains);
    mock_region->SetPosition(kExpectedPosition);
    mock_region->SetOverlappingType(kExpectedOverlapping);
    dut_ = std::move(mock_region);
  }
  const bool kExpectedContains{true};
  const Vector3 kExpectedPosition{1., 2., 3.};
  const api::BoundingRegion<Vector3>::OverlappingType kExpectedOverlapping{
      api::BoundingRegion<Vector3>::OverlappingType::kContained};
  std::unique_ptr<api::BoundingRegion<Vector3>> dut_;
};

TEST_F(BoundingRegionTest, API) {
  MockBoundaryRegion* mock_region{nullptr};
  ASSERT_NE(nullptr, mock_region = dynamic_cast<MockBoundaryRegion*>(dut_.get()));

  EXPECT_EQ(kExpectedContains, dut_->Contains(Vector3::Ones()));
  EXPECT_EQ(kExpectedPosition, dut_->position());
  EXPECT_EQ(kExpectedOverlapping, dut_->Overlaps(MockBoundaryRegion()));
}

}  // namespace
}  // namespace test
}  // namespace object
}  // namespace maliput
