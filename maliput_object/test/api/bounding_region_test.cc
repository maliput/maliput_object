// Copyright 2022 Toyota Research Institute
#include "maliput_object/api/bounding_region.h"

#include <memory>

#include <gtest/gtest.h>
#include <maliput/math/vector.h>

#include "maliput_object/test_utilities/mock.h"

namespace maliput {
namespace object {
namespace test {
namespace {

using maliput::math::Vector3;

// Tests API interface.
class BoundingRegionTest : public ::testing::Test {
 public:
  void SetUp() override {
    auto mock_region = std::make_unique<test_utilities::MockBoundingRegion>();
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
  test_utilities::MockBoundingRegion* mock_region{nullptr};
  ASSERT_NE(nullptr, mock_region = dynamic_cast<test_utilities::MockBoundingRegion*>(dut_.get()));

  EXPECT_EQ(kExpectedContains, dut_->Contains(Vector3::Ones()));
  EXPECT_EQ(kExpectedPosition, dut_->position());
  EXPECT_EQ(kExpectedOverlapping, dut_->Overlaps(test_utilities::MockBoundingRegion()));
}

}  // namespace
}  // namespace test
}  // namespace object
}  // namespace maliput
