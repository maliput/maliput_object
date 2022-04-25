// Copyright 2022 Toyota Research Institute
#include "maliput_object/api/object.h"

#include <memory>
#include <optional>
#include <string>

#include <gtest/gtest.h>
#include <maliput/math/vector.h>

#include "maliput_object/api/bounding_region.h"
#include "maliput_object/test_utilities/mock.h"

namespace maliput {
namespace object {
namespace api {
namespace test {
namespace {

using maliput::math::Vector3;

class ObjectTest : public ::testing::Test {
 public:
  const Object<Vector3>::Id kId{"ObjectTest"};
  const Vector3 kExpectedPosition{1., 2., 3.};
  const api::OverlappingType kExpectedOverlapping{api::OverlappingType::kContained};
  const std::map<std::string, std::string> kExpectedProperties{{"Key1", "Value1"}, {"Key2", "Value2"}};
  std::unique_ptr<test_utilities::MockBoundingRegion> region_{std::make_unique<test_utilities::MockBoundingRegion>()};
  const test_utilities::MockBoundingRegion* region_ptr_{region_.get()};
};

TEST_F(ObjectTest, Constructor) { EXPECT_NO_THROW(Object<Vector3>(kId, {}, std::move(region_))); }

TEST_F(ObjectTest, API) {
  const api::Object<Vector3> dut{kId, kExpectedProperties, std::move(region_)};
  ASSERT_EQ(kId, dut.id());

  EXPECT_CALL(*region_ptr_, DoOverlaps(::testing::_)).Times(1).WillOnce(::testing::Return(kExpectedOverlapping));
  EXPECT_CALL(*region_ptr_, do_position()).Times(1).WillOnce(::testing::ReturnRef(kExpectedPosition));
  ASSERT_EQ(kExpectedOverlapping, dut.bounding_region().Overlaps(test_utilities::MockBoundingRegion{}));
  ASSERT_EQ(kExpectedPosition, dut.position());

  ASSERT_EQ(kExpectedProperties, dut.get_properties());
  const std::string kValidPropertyKey{kExpectedProperties.begin()->first};
  ASSERT_EQ(kExpectedProperties.at(kValidPropertyKey), dut.get_property(kValidPropertyKey));
  const std::string kInvalidPropertyKey{"invalid_key"};
  ASSERT_EQ(std::nullopt, dut.get_property(kInvalidPropertyKey));
}

}  // namespace
}  // namespace test
}  // namespace api
}  // namespace object
}  // namespace maliput
