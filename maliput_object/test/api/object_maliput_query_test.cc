// Copyright 2022 Toyota Research Institute
#include "maliput_object/api/object_maliput_query.h"

#include <memory>
#include <optional>
#include <string>

#include <gtest/gtest.h>
#include <maliput/api/lane.h>
#include <maliput/api/lane_data.h>
#include <maliput/api/road_network.h>
#include <maliput/math/vector.h>
#include <maliput/test_utilities/mock.h>

#include "maliput_object/api/bounding_region.h"
#include "maliput_object/api/object.h"
#include "maliput_object/test_utilities/mock.h"

namespace maliput {
namespace object {
namespace api {
namespace test {
namespace {

using maliput::math::Vector3;

class ObjectMaliputQueryTest : public ::testing::Test {
 public:
  void SetUp() override {}

  std::unique_ptr<api::ObjectBook<Vector3>> object_book_ = std::make_unique<test_utilities::MockObjectBook>();
  std::unique_ptr<maliput::api::RoadNetwork> road_network_ = maliput::api::test::CreateRoadNetwork();
  std::unique_ptr<maliput::api::Lane> lane_1_ = maliput::api::test::CreateLane(maliput::api::LaneId{"lane_1"});
  std::unique_ptr<maliput::api::Lane> lane_2_ = maliput::api::test::CreateLane(maliput::api::LaneId{"lane_2"});
  std::unique_ptr<maliput::api::Lane> lane_3_ = maliput::api::test::CreateLane(maliput::api::LaneId{"lane_3"});

  const api::ObjectBook<maliput::math::Vector3>* kExpectedObjectBook{object_book_.get()};
  const maliput::api::RoadNetwork* kExpectedRoadNetwork{road_network_.get()};
  std::vector<const maliput::api::Lane*> kExpectedOverlappingsLanesIn{lane_1_.get(), lane_2_.get()};
  std::vector<const maliput::api::Lane*> kExpectedOverlappingsLanesInByType{lane_2_.get(), lane_3_.get()};
  std::optional<const maliput::api::LaneSRoute> kExpectedRoute{
      std::make_optional<>(maliput::api::test::CreateLaneSRoute())};
};

// Tests ObjectBook API.
TEST_F(ObjectMaliputQueryTest, API) {
  std::unique_ptr<api::ObjectMaliputQuery> dut = std::make_unique<test_utilities::MockObjectMaliputQuery>(
      kExpectedObjectBook, kExpectedRoadNetwork, kExpectedOverlappingsLanesIn, kExpectedOverlappingsLanesInByType,
      kExpectedRoute.value());

  ASSERT_NE(dut, nullptr);
  EXPECT_EQ(kExpectedObjectBook, dut->object_book());
  EXPECT_EQ(kExpectedRoadNetwork, dut->road_network());

  const Object<Vector3> kObject{
      Object<Vector3>::Id{"test_object"}, {}, std::make_unique<test_utilities::MockBoundingRegion>()};
  EXPECT_EQ(kExpectedOverlappingsLanesIn, dut->FindOverlappingLanesIn(&kObject));
  EXPECT_EQ(kExpectedOverlappingsLanesInByType,
            dut->FindOverlappingLanesIn(&kObject, api::BoundingRegion<Vector3>::OverlappingType::kIntersected));
  EXPECT_EQ(kExpectedRoute.value().length(), dut->Route(&kObject, &kObject).value().length());
}

}  // namespace
}  // namespace test
}  // namespace api
}  // namespace object
}  // namespace maliput
