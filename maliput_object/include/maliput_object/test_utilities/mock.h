// Copyright 2022 Toyota Research Institute
#include <memory>
#include <unordered_map>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <maliput/api/lane.h>
#include <maliput/api/road_network.h>
#include <maliput/math/vector.h>

#include "maliput_object/api/bounding_region.h"
#include "maliput_object/api/object.h"
#include "maliput_object/api/object_book.h"
#include "maliput_object/api/object_query.h"
#include "maliput_object/api/overlapping_type.h"

namespace maliput {
namespace object {
namespace test_utilities {

class MockBoundingRegion : public api::BoundingRegion<maliput::math::Vector3> {
 public:
  MOCK_METHOD((const maliput::math::Vector3&), do_position, (), (const, override));
  MOCK_METHOD((bool), DoContains, (const maliput::math::Vector3&), (const, override));
  MOCK_METHOD((api::OverlappingType), DoOverlaps, (const BoundingRegion<maliput::math::Vector3>&), (const, override));
};

template <typename Coordinate>
class MockObjectBook : public api::ObjectBook<Coordinate> {
 public:
  MockObjectBook() = default;
  MOCK_METHOD((std::unordered_map<typename api::Object<Coordinate>::Id, api::Object<Coordinate>*>), do_objects, (),
              (const, override));
  MOCK_METHOD((api::Object<Coordinate>*), DoFindById, (const typename api::Object<Coordinate>::Id&), (const, override));
  MOCK_METHOD((std::vector<api::Object<Coordinate>*>), DoFindByPredicate,
              (std::function<bool(const api::Object<Coordinate>*)>), (const, override));
  MOCK_METHOD((std::vector<api::Object<Coordinate>*>), DoFindOverlappingIn,
              (const api::BoundingRegion<Coordinate>&, const api::OverlappingType&), (const, override));
};

class MockObjectQuery : public api::ObjectQuery {
 public:
  MOCK_METHOD((std::vector<const maliput::api::Lane*>), DoFindOverlappingLanesIn,
              (const api::Object<maliput::math::Vector3>*), (const, override));
  MOCK_METHOD((std::vector<const maliput::api::Lane*>), DoFindOverlappingLanesIn,
              (const api::Object<maliput::math::Vector3>*, const api::OverlappingType&), (const, override));
  MOCK_METHOD((std::optional<const maliput::api::LaneSRoute>), DoRoute,
              (const api::Object<maliput::math::Vector3>*, const api::Object<maliput::math::Vector3>*),
              (const, override));
  MOCK_METHOD((const api::ObjectBook<maliput::math::Vector3>*), do_object_book, (), (const, override));
  MOCK_METHOD((const maliput::api::RoadNetwork*), do_road_network, (), (const, override));
};

}  // namespace test_utilities
}  // namespace object
}  // namespace maliput
