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
  const bool kExpectedContains{true};
  const Vector3 kExpectedPosition{1., 2., 3.};
  const api::OverlappingType kExpectedOverlapping{api::OverlappingType::kContained};
  const std::unique_ptr<api::BoundingRegion<Vector3>> dut_ = std::make_unique<test_utilities::MockBoundingRegion>();
};

TEST_F(BoundingRegionTest, API) {
  test_utilities::MockBoundingRegion* mock_region{nullptr};
  ASSERT_NE(nullptr, mock_region = dynamic_cast<test_utilities::MockBoundingRegion*>(dut_.get()));

  EXPECT_CALL(*mock_region, do_position()).Times(1).WillOnce(::testing::ReturnRef(kExpectedPosition));
  EXPECT_CALL(*mock_region, DoContains(Vector3::Ones())).Times(1).WillOnce(::testing::Return(kExpectedContains));
  EXPECT_CALL(*mock_region, DoOverlaps(::testing::_)).Times(1).WillOnce(::testing::Return(kExpectedOverlapping));

  EXPECT_EQ(kExpectedPosition, dut_->position());
  EXPECT_EQ(kExpectedContains, dut_->Contains(Vector3::Ones()));
  EXPECT_EQ(kExpectedOverlapping, dut_->Overlaps(*dut_));
}

}  // namespace
}  // namespace test
}  // namespace object
}  // namespace maliput
