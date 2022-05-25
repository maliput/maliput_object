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
