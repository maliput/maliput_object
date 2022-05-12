// Copyright 2022 Toyota Research Institute
#include "maliput_object/loader/loader.h"

#include <stdexcept>
#include <string>
#include <unordered_map>

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <gtest/gtest.h>
#include <maliput/common/assertion_error.h>
#include <maliput/math/roll_pitch_yaw.h>
#include <maliput/math/vector.h>

#include "maliput_object/api/bounding_region.h"
#include "maliput_object/api/object.h"
#include "maliput_object/api/object_book.h"
#include "maliput_object/base/bounding_box.h"

namespace maliput {
namespace object {
namespace loader {
namespace test {
namespace {

using maliput::object::BoundingBox;
using maliput::object::api::BoundingRegion;
using maliput::object::api::Object;

// @{ Asserts that no file or no string with YAML content throws.
TEST(LoadTest, EmptyString) { ASSERT_THROW(Load(""), std::runtime_error); }

TEST(LoadFileTest, EmptyString) { ASSERT_THROW(LoadFile(""), std::runtime_error); }
// @}

// Test structure to verify schema errors.
struct SchemaParserTestCase {
  std::string constraint{};
  std::string yaml_under_test{};
};

// Just to improve gtest error report.
std::ostream& operator<<(std::ostream& os, const SchemaParserTestCase& test_case) {
  os << "{ constraint: " << test_case.constraint << ", yaml_under_test: " << test_case.yaml_under_test << "}";
  return os;
}

// Returns a list of test cases for the schema assertions.
std::vector<SchemaParserTestCase> GetYamlsWithSchemaErrors() {
  return {
      {"no maliput objects key", fmt::format(
                                     R"R(---
a_maliput_key: \"Hello world!\"
anoter_maliput_key:
  still_not_valid_key:
    - 1
    - 2
    - 3 
)R")},
      {"maliput_objects must be a mapping", fmt::format(
                                                R"R(---
maliput_objects:
  - 1
  - 2
)R")},
      {"objects must contain a bounding_region", fmt::format(
                                                     R"R(---
maliput_objects:
  an_object:
    not_a_bounding_region:
      position: [1, 2, 3]
      rotation: [0.4, 0.5, 0.6]
      type: box
      box_size: [0.7, 0.8, 0.9]
    properties:
      a_key: a_value
)R")},
      {"bounding_region must contain a position", fmt::format(
                                                      R"R(---
maliput_objects:
  an_object:
    bounding_region:
      not_a_position: [1, 2, 3]
      rotation: [0.4, 0.5, 0.6]
      type: box
      box_size: [0.7, 0.8, 0.9]
    properties:
      a_key: a_value
)R")},
      {"bounding_region must contain a rotation", fmt::format(
                                                      R"R(---
maliput_objects:
  an_object:
    bounding_region:
      not_a_position: [1, 2, 3]
      rotation: [0.4, 0.5, 0.6]
      type: box
      box_size: [0.7, 0.8, 0.9]
    properties:
      a_key: a_value
)R")},
      {"bounding_region must contain a type", fmt::format(
                                                  R"R(---
maliput_objects:
  an_object:
    bounding_region:
      position: [1, 2, 3]
      rotation: [0.4, 0.5, 0.6]
      not_a_type: box
      box_size: [0.7, 0.8, 0.9]
    properties:
      a_key: a_value
)R")},
      {"bounding_region must contain a type and must be a box", fmt::format(
                                                                    R"R(---
maliput_objects:
  an_object:
    bounding_region:
      position: [1, 2, 3]
      rotation: [0.4, 0.5, 0.6]
      type: not_box
      box_size: [0.7, 0.8, 0.9]
    properties:
      a_key: a_value
)R")},
      {"bounding_region must contain box_size", fmt::format(
                                                    R"R(---
maliput_objects:
  an_object:
    bounding_region:
      position: [1, 2, 3]
      rotation: [0.4, 0.5, 0.6]
      type: box
      not_box_size: [0.7, 0.8, 0.9]
    properties:
      a_key: a_value
)R")},
      {"position has no three elements", fmt::format(
                                             R"R(---
maliput_objects:
  an_object:
    bounding_region:
      position: [1., 2.]
      rotation: [0.4, 0.5, 0.6]
      type: box
      box_size: [0.7, 0.8, 0.9]
    properties:
      a_key: a_value
)R")},
      {"rotation has no three elements", fmt::format(
                                             R"R(---
maliput_objects:
  an_object:
    bounding_region:
      position: [1., 2., 3.]
      rotation: [0.4, 0.5, 0.6, 0.666]
      type: box
      box_size: [0.7, 0.8, 0.9]
    properties:
      a_key: a_value
)R")},
      {"box_size has no three elements", fmt::format(
                                             R"R(---
maliput_objects:
  an_object:
    bounding_region:
      position: [1., 2., 3.]
      rotation: [0.4, 0.5, 0.6, 0.666]
      type: box
      box_size: [0.7]
    properties:
      a_key: a_value
)R")},
  };
}

class SchemaParserCheckTest : public ::testing::TestWithParam<SchemaParserTestCase> {};

TEST_P(SchemaParserCheckTest, AssertsThatItThrowsWhenSchemaIsIncorrect) {
  ASSERT_THROW(Load(GetParam().yaml_under_test), maliput::common::assertion_error);
}

INSTANTIATE_TEST_CASE_P(SchemaParserCheckTestGroup, SchemaParserCheckTest,
                        ::testing::ValuesIn(GetYamlsWithSchemaErrors()));

TEST(LoadTest, OneValidObject) {
  const std::string kObjectId{"object_id"};
  const double kX{1.0};
  const double kY{2.0};
  const double kZ{3.0};
  const double kRoll{.4};
  const double kPitch{.5};
  const double kYaw{.6};
  const double kLength{7.};
  const double kWidth{8.};
  const double kHeight{9.};
  const std::string kKey{"a_key"};
  const std::string kValue{"a value"};
  const std::string dut = fmt::format(
      R"R(---
maliput_objects:
  {}:
    bounding_region:
      position: [{}, {}, {}]
      rotation: [{}, {}, {}]
      type: box
      box_size: [{}, {}, {}]
    properties:
      {}: "{}" 
)R",
      kObjectId, kX, kY, kZ, kRoll, kPitch, kYaw, kLength, kWidth, kHeight, kKey, kValue);

  std::unique_ptr<api::ObjectBook<maliput::math::Vector3>> object_book;
  object_book = Load(dut);
  ASSERT_NE(nullptr, object_book);
  ASSERT_EQ(1u, object_book->objects().size());

  const Object<maliput::math::Vector3>::Id id{kObjectId};
  Object<maliput::math::Vector3>* object = object_book->FindById(id);
  ASSERT_NE(nullptr, object);
  ASSERT_EQ(id, object->id());
  ASSERT_DOUBLE_EQ(kX, object->position().x());
  ASSERT_DOUBLE_EQ(kY, object->position().y());
  ASSERT_DOUBLE_EQ(kZ, object->position().z());
  ASSERT_EQ(1u, object->get_properties().size());
  ASSERT_EQ(kValue, object->get_property(kKey));

  const BoundingRegion<maliput::math::Vector3>& bounding_region = object->bounding_region();
  const BoundingBox* bounding_box = dynamic_cast<const BoundingBox*>(&bounding_region);
  ASSERT_NE(nullptr, bounding_box);
  ASSERT_DOUBLE_EQ(kX, bounding_box->position().x());
  ASSERT_DOUBLE_EQ(kY, bounding_box->position().y());
  ASSERT_DOUBLE_EQ(kZ, bounding_box->position().z());
  ASSERT_DOUBLE_EQ(kRoll, bounding_box->get_orientation().roll_angle());
  ASSERT_DOUBLE_EQ(kPitch, bounding_box->get_orientation().pitch_angle());
  ASSERT_DOUBLE_EQ(kYaw, bounding_box->get_orientation().yaw_angle());
  // TODO(#15): verify vertices to assert box size.
}

}  // namespace
}  // namespace test
}  // namespace loader
}  // namespace object
}  // namespace maliput