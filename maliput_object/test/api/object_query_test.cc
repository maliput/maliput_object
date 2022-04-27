// Copyright 2022 Toyota Research Institute
#include "maliput_object/api/object_query.h"

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

class ObjectQueryTest : public ::testing::Test {
 public:
  const std::unique_ptr<api::ObjectBook<Vector3>> object_book_ =
      std::make_unique<test_utilities::MockObjectBook<Vector3>>();
  const std::unique_ptr<maliput::api::RoadNetwork> road_network_ = maliput::api::test::CreateRoadNetwork();
  const std::unique_ptr<maliput::api::Lane> lane_ = maliput::api::test::CreateLane(maliput::api::LaneId{"lane_1"});
  const Object<Vector3> kObject{
      Object<Vector3>::Id{"test_object"}, {}, std::make_unique<test_utilities::MockBoundingRegion>()};
  const api::OverlappingType kOverlappingType{api::OverlappingType::kContained};
  const api::ObjectBook<maliput::math::Vector3>* kExpectedObjectBook{object_book_.get()};
  const maliput::api::RoadNetwork* kExpectedRoadNetwork{road_network_.get()};
  const std::vector<const maliput::api::Lane*> kExpectedOverlappingsLanesIn{lane_.get()};
  const std::vector<const maliput::api::Lane*> kExpectedOverlappingsLanesInByType{kExpectedOverlappingsLanesIn};
  const std::optional<const maliput::api::LaneSRoute> kExpectedRoute{
      std::make_optional<>(maliput::api::test::CreateLaneSRoute())};
};

// Tests ObjectBook API.
TEST_F(ObjectQueryTest, API) {
  const test_utilities::MockObjectQuery dut;
  EXPECT_CALL(dut, do_road_network()).Times(1).WillOnce(::testing::Return(kExpectedRoadNetwork));
  EXPECT_CALL(dut, do_object_book()).Times(1).WillOnce(::testing::Return(kExpectedObjectBook));
  EXPECT_CALL(dut, DoFindOverlappingLanesIn(&kObject))
      .Times(1)
      .WillOnce(::testing::Return(kExpectedOverlappingsLanesIn));
  EXPECT_CALL(dut, DoFindOverlappingLanesIn(&kObject, kOverlappingType))
      .Times(1)
      .WillOnce(::testing::Return(kExpectedOverlappingsLanesInByType));
  EXPECT_CALL(dut, DoRoute(&kObject, &kObject)).Times(1).WillOnce(::testing::Return(kExpectedRoute));

  EXPECT_EQ(kExpectedObjectBook, dut.object_book());
  EXPECT_EQ(kExpectedRoadNetwork, dut.road_network());
  EXPECT_EQ(kExpectedOverlappingsLanesIn, dut.FindOverlappingLanesIn(&kObject));
  EXPECT_EQ(kExpectedOverlappingsLanesInByType, dut.FindOverlappingLanesIn(&kObject, kOverlappingType));
  EXPECT_EQ(kExpectedRoute.value().length(), dut.Route(&kObject, &kObject).value().length());
}

}  // namespace
}  // namespace test
}  // namespace api
}  // namespace object
}  // namespace maliput
