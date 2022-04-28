// Copyright 2022 Toyota Research Institute
#include "maliput_object/api/overlapping_type.h"

#include <gtest/gtest.h>

namespace maliput {
namespace object {
namespace api {
namespace test {
namespace {

TEST(OverlappingTypeTest, TestOverlappingType) {
  EXPECT_EQ(OverlappingType::kIntersected, OverlappingType::kContained & OverlappingType::kIntersected);
  EXPECT_EQ(OverlappingType::kDisjointed, OverlappingType::kContained & OverlappingType::kDisjointed);
  EXPECT_EQ(OverlappingType::kContained, (OverlappingType::kContained | OverlappingType::kIntersected));
}

}  // namespace
}  // namespace test
}  // namespace api
}  // namespace object
}  // namespace maliput
