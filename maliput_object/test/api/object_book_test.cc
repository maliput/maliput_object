// Copyright 2022 Toyota Research Institute
#include "maliput_object/api/object_book.h"

#include <memory>
#include <optional>
#include <string>

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

class ObjectBookTest : public ::testing::Test {
 public:
  void SetUp() override {}

  static std::unique_ptr<Object<Vector3>> CreateObject(const std::string& id,
                                                       std::map<std::string, std::string> properties,
                                                       std::unique_ptr<BoundingRegion<Vector3>> region) {
    return std::make_unique<Object<Vector3>>(Object<Vector3>::Id{id}, properties, std::move(region));
  }

  const std::map<std::string, std::string> kArbitraryProperties{{"property_1", "value_1"}, {"property_2", "value_2"}};
  std::unique_ptr<Object<Vector3>> object_1 =
      CreateObject("object_1", kArbitraryProperties, std::make_unique<test_utilities::MockBoundingRegion>());
  std::unique_ptr<Object<Vector3>> object_2 =
      CreateObject("object_2", kArbitraryProperties, std::make_unique<test_utilities::MockBoundingRegion>());
  std::unique_ptr<Object<Vector3>> object_3 =
      CreateObject("object_3", kArbitraryProperties, std::make_unique<test_utilities::MockBoundingRegion>());
  std::unique_ptr<Object<Vector3>> object_4 =
      CreateObject("object_4", kArbitraryProperties, std::make_unique<test_utilities::MockBoundingRegion>());

  const std::unordered_map<api::Object<Vector3>::Id, api::Object<Vector3>*> expected_objects{
      {object_1->id(), object_1.get()},
      {object_2->id(), object_2.get()},
      {object_3->id(), object_3.get()},
      {object_4->id(), object_4.get()}};
  api::Object<Vector3>* expected_object_by_id{object_1.get()};
  const std::vector<api::Object<Vector3>*> expected_objects_by_predicate{{object_2.get(), object_3.get()}};
  const std::vector<api::Object<Vector3>*> expected_objects_by_overlapping{{object_3.get(), object_4.get()}};
};

// Tests ObjectBook API.
TEST_F(ObjectBookTest, API) {
  std::unique_ptr<ObjectBook<Vector3>> dut = std::make_unique<test_utilities::MockObjectBook>(
      expected_objects, expected_object_by_id, expected_objects_by_predicate, expected_objects_by_overlapping);
  EXPECT_EQ(expected_objects, dut->objects());
  EXPECT_EQ(expected_object_by_id, dut->FindBy(object_1->id()));
  EXPECT_EQ(expected_objects_by_predicate, dut->FindBy([](const Object<Vector3>*) -> bool {
    return true; /* My predicate */
  }));
  const auto kArbitraryBoundingRegion = std::make_unique<test_utilities::MockBoundingRegion>();
  EXPECT_EQ(expected_objects_by_overlapping, dut->FindOverlappingIn(*kArbitraryBoundingRegion));
  EXPECT_EQ(expected_objects_by_overlapping,
            dut->FindOverlappingIn(*kArbitraryBoundingRegion, BoundingRegion<Vector3>::OverlappingType::kIntersected));
}

}  // namespace
}  // namespace test
}  // namespace api
}  // namespace object
}  // namespace maliput
