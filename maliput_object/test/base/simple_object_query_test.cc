#include "maliput_object/base/simple_object_query.h"

#include <memory>

#include <gtest/gtest.h>
#include <maliput/api/road_network.h>
#include <maliput/common/assertion_error.h>
#include <maliput/test_utilities/mock.h>

#include "maliput_object/api/object.h"
#include "maliput_object/test_utilities/mock.h"

namespace maliput {
namespace object {
namespace test {
namespace {

class SimpleObjectQueryTest : public ::testing::Test {
 public:
  std::unique_ptr<maliput::api::RoadNetwork> road_network_ = maliput::api::test::CreateRoadNetwork();
  std::unique_ptr<api::ObjectBook<maliput::math::Vector3>> object_book_ =
      std::make_unique<test_utilities::MockObjectBook<maliput::math::Vector3>>();
};

TEST_F(SimpleObjectQueryTest, Constructor) {
  EXPECT_THROW(SimpleObjectQuery(nullptr, object_book_.get()), maliput::common::assertion_error);
  EXPECT_THROW(SimpleObjectQuery(road_network_.get(), nullptr), maliput::common::assertion_error);
  EXPECT_NO_THROW(SimpleObjectQuery(road_network_.get(), object_book_.get()));
}

TEST_F(SimpleObjectQueryTest, Getters) {
  SimpleObjectQuery dut(road_network_.get(), object_book_.get());
  EXPECT_EQ(road_network_.get(), dut.road_network());
  EXPECT_EQ(object_book_.get(), dut.object_book());
}

// Route and FindOverlappingIn methods are easier to test via integration tests. They are tested at
// maliput_integration_tests package.

}  // namespace
}  // namespace test
}  // namespace object
}  // namespace maliput
