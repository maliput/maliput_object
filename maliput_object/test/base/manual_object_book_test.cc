// Copyright 2022 Toyota Research Institute
#include "maliput_object/base/manual_object_book.h"

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
#include "maliput_object/api/object_book.h"
#include "maliput_object/test_utilities/mock.h"

namespace maliput {
namespace object {
namespace test {
namespace {

using maliput::math::Vector3;

class ManualObjectBookTest : public ::testing::Test {
 public:
  void SetUp() override {
    EXPECT_EQ(0, static_cast<int>(dut_.objects().size()));
    dut_.AddObject(std::move(kObjectA));
    dut_.AddObject(std::move(kObjectB));
    EXPECT_EQ(2, static_cast<int>(dut_.objects().size()));
  }
  const api::Object<Vector3>::Id kIdA{"id_a"};
  const api::Object<Vector3>::Id kIdB{"id_b"};
  const std::string kPropertyA{"PropertyA"};
  const std::string kPropertyB{"PropertyB"};
  // Object A.
  std::unique_ptr<test_utilities::MockBoundingRegion> kRegionA = std::make_unique<test_utilities::MockBoundingRegion>();
  const test_utilities::MockBoundingRegion* kRegionAPtr{kRegionA.get()};
  std::unique_ptr<api::Object<Vector3>> kObjectA = std::make_unique<api::Object<Vector3>>(
      kIdA, std::map<std::string, std::string>{{kPropertyA, "DescriptionA"}}, std::move(kRegionA));
  const api::Object<Vector3>* kObjectAPtr{kObjectA.get()};

  // Object B.
  std::unique_ptr<test_utilities::MockBoundingRegion> kRegionB = std::make_unique<test_utilities::MockBoundingRegion>();
  const test_utilities::MockBoundingRegion* kRegionBPtr{kRegionB.get()};
  std::unique_ptr<api::Object<Vector3>> kObjectB = std::make_unique<api::Object<Vector3>>(
      kIdB, std::map<std::string, std::string>{{kPropertyB, "DescriptionB"}}, std::move(kRegionB));
  const api::Object<Vector3>* kObjectBPtr{kObjectB.get()};

  ManualObjectBook<Vector3> dut_;
};

TEST_F(ManualObjectBookTest, ManualObjectBook) {
  // Find by Id.
  EXPECT_EQ(kObjectAPtr->id(), dut_.FindById(kIdA)->id());
  // Find by predicate.
  const auto objects_by_predicate = dut_.FindByPredicate(
      [this](const api::Object<Vector3>* object) { return object->get_property(this->kPropertyB).has_value(); });
  ASSERT_EQ(1, static_cast<int>(objects_by_predicate.size()));
  EXPECT_EQ(kObjectBPtr->id(), objects_by_predicate.front()->id());

  // Find by bounding region.
  EXPECT_CALL(*kRegionAPtr, DoOverlaps(::testing::_))
      .Times(1)
      .WillOnce(::testing::Return(api::OverlappingType::kContained));
  EXPECT_CALL(*kRegionBPtr, DoOverlaps(::testing::_))
      .Times(1)
      .WillOnce(::testing::Return(api::OverlappingType::kIntersected));
  const auto objects_by_region = dut_.FindOverlappingIn(*kRegionAPtr, api::OverlappingType::kContained);
  ASSERT_EQ(1, static_cast<int>(objects_by_region.size()));
  EXPECT_EQ(kObjectAPtr->id(), objects_by_region.front()->id());

  dut_.RemoveObject(kObjectAPtr->id());
  EXPECT_EQ(1, static_cast<int>(dut_.objects().size()));
}

}  // namespace
}  // namespace test
}  // namespace object
}  // namespace maliput
