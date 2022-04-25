// Copyright 2022 Toyota Research Institute
#include "maliput_object/api/overlapping_type.h"

#include <gtest/gtest.h>

namespace maliput {
namespace object {
namespace api {
namespace test {
namespace {

TEST(OverlappingTypeTest, TestOverlappingType) {
  EXPECT_EQ(OverlappingType::kContained, OverlappingType::kContained & OverlappingType::kAll);
  EXPECT_EQ(OverlappingType::kIntersected, OverlappingType::kIntersected & OverlappingType::kAll);
  EXPECT_EQ(OverlappingType::kAll, OverlappingType::kIntersected | OverlappingType::kContained);
}

}  // namespace
}  // namespace test
}  // namespace api
}  // namespace object
}  // namespace maliput
