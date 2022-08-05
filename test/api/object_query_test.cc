// BSD 3-Clause License
//
// Copyright (c) 2022, Woven Planet. All rights reserved.
// Copyright (c) 2022, Toyota Research Institute. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include "maliput_object/api/object_query.h"

#include <memory>
#include <optional>
#include <string>

#include <gtest/gtest.h>
#include <maliput/api/lane.h>
#include <maliput/api/lane_data.h>
#include <maliput/api/road_network.h>
#include <maliput/math/bounding_region.h>
#include <maliput/math/vector.h>
#include <maliput/test_utilities/mock.h>
#include <maliput/test_utilities/mock_math.h>

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
      Object<Vector3>::Id{"test_object"}, {}, std::make_unique<maliput::math::test::MockBoundingRegion>()};
  const maliput::math::OverlappingType kOverlappingType{maliput::math::OverlappingType::kContained};
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
