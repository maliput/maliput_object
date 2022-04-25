// Copyright 2022 Toyota Research Institute
#include "maliput_object/api/object_book.h"

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <maliput/math/vector.h>

#include "maliput_object/api/bounding_region.h"
#include "maliput_object/api/object.h"
#include "maliput_object/test_utilities/mock.h"

namespace maliput {
namespace object {
namespace api {
namespace test {
namespace {

using maliput::math::Vector3;

TEST(ObjectBookTest, API) {
  test_utilities::MockObjectBook<Vector3> dut;
  const api::Object<Vector3>::Id kId{"id"};
  const test_utilities::MockBoundingRegion kBoundingRegion{};
  const api::OverlappingType kOverlappingType{OverlappingType::kContained | OverlappingType::kIntersected};
  const std::unordered_map<api::Object<Vector3>::Id, api::Object<Vector3>*> kExpectedObjects{};
  std::unique_ptr<api::Object<Vector3>> kExpectedObjectById = std::make_unique<api::Object<Vector3>>(
      kId, std::map<std::string, std::string>{}, std::make_unique<test_utilities::MockBoundingRegion>());
  const std::vector<api::Object<Vector3>*> kExpectedObjectsByPredicate{kExpectedObjectById.get()};
  const std::vector<api::Object<Vector3>*> kExpectedObjectsByOverlapping{kExpectedObjectsByPredicate};
  const std::function<bool(const api::Object<Vector3>*)> kPredicate = [](const api::Object<Vector3>*) { return true; };
  EXPECT_CALL(dut, do_objects()).Times(1).WillOnce(::testing::Return(kExpectedObjects));
  EXPECT_CALL(dut, DoFindById(kId)).Times(1).WillOnce(::testing::Return(kExpectedObjectById.get()));
  EXPECT_CALL(dut, DoFindByPredicate(::testing::_)).Times(1).WillOnce(::testing::Return(kExpectedObjectsByPredicate));
  EXPECT_CALL(dut, DoFindOverlappingIn(::testing::_, kOverlappingType))
      .Times(1)
      .WillOnce(::testing::Return(kExpectedObjectsByOverlapping));
  EXPECT_EQ(kExpectedObjects, dut.objects());
  EXPECT_EQ(kExpectedObjectById.get(), dut.FindById(kId));
  EXPECT_EQ(kExpectedObjectsByPredicate, dut.FindByPredicate(kPredicate));
  EXPECT_EQ(kExpectedObjectsByOverlapping, dut.FindOverlappingIn(kBoundingRegion, kOverlappingType));
}

}  // namespace
}  // namespace test
}  // namespace api
}  // namespace object
}  // namespace maliput
