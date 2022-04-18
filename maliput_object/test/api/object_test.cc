// Copyright 2022 Toyota Research Institute
#include "maliput_object/api/object.h"

#include <memory>
#include <optional>
#include <string>

#include <gtest/gtest.h>
#include <maliput/math/vector.h>

#include "maliput_object/api/bounding_region.h"

namespace maliput {
namespace object {
namespace test {
namespace {

using maliput::math::Vector3;

// Mocks BoundaryRegion class hardcoding the api's return values.
class MockBoundingRegion : public api::BoundingRegion<Vector3> {
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

class ObjectTest : public ::testing::Test {
 public:
  void SetUp() override {
    auto mock_region = std::make_unique<MockBoundingRegion>();
    mock_region->SetPosition(kExpectedPosition);
    mock_region->SetOverlappingType(kExpectedOverlapping);
    region_ = std::move(mock_region);
  }

  const api::Object<Vector3>::Id kId{"ObjectTest"};
  const Vector3 kExpectedPosition{1., 2., 3.};
  const api::BoundingRegion<Vector3>::OverlappingType kExpectedOverlapping{
      api::BoundingRegion<Vector3>::OverlappingType::kContained};
  const std::map<std::string, std::string> kExpectedProperties{{"Key1", "Value1"}, {"Key2", "Value2"}};
  std::unique_ptr<api::BoundingRegion<Vector3>> region_;
};

TEST_F(ObjectTest, Constructor) { EXPECT_NO_THROW(api::Object<Vector3>(kId, {}, std::move(region_))); }

TEST_F(ObjectTest, API) {
  const api::Object<Vector3> dut{kId, kExpectedProperties, std::move(region_)};
  EXPECT_EQ(kId, dut.id());
  EXPECT_EQ(kExpectedOverlapping, dut.bounding_region().Overlaps(MockBoundingRegion{}));
  EXPECT_EQ(kExpectedPosition, dut.position());
  EXPECT_EQ(kExpectedProperties, dut.get_properties());
  const std::string kValidPropertyKey{kExpectedProperties.begin()->first};
  EXPECT_EQ(kExpectedProperties.at(kValidPropertyKey), dut.get_property(kValidPropertyKey));
  const std::string kInvalidPropertyKey{"invalid_key"};
  EXPECT_EQ(std::nullopt, dut.get_property(kInvalidPropertyKey));
}

}  // namespace
}  // namespace test
}  // namespace object
}  // namespace maliput
