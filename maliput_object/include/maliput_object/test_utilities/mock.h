// Copyright 2022 Toyota Research Institute
#include <memory>
#include <unordered_map>

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

/// Mocks ObjectQuery class hardcoding the api's return values.
class MockObjectQuery : public api::ObjectQuery {
 public:
  MockObjectQuery() = default;

  /// Constructs a MockObjectQuery.
  /// @param object_book ObjectBook to be returned when calling object_book() method.
  /// @param road_network RoadNetwork to be returned when calling road_network() method.
  /// @param lanes_by_object Lanes to be returned when calling FindOverlappingLanesIn() method.
  /// @param lanes_by_overlapping_type Lanes to be returned when calling FindOverlappingLanesIn() method with an
  /// overlapping type.
  /// @param lane_s_route LaneSRoute to be returned when calling Route() method.
  MockObjectQuery(const api::ObjectBook<maliput::math::Vector3>* object_book,
                  const maliput::api::RoadNetwork* road_network,
                  const std::vector<const maliput::api::Lane*>& lanes_by_object,
                  const std::vector<const maliput::api::Lane*>& lanes_by_overlapping_type,
                  const maliput::api::LaneSRoute& lane_s_route)
      : object_book_(object_book),
        road_network_(road_network),
        lanes_by_object_(lanes_by_object),
        lanes_by_overlapping_type_(lanes_by_overlapping_type),
        lane_s_route_(lane_s_route) {}

  /// @param object_book ObjectBook to be returned when calling object_book() method.
  void SetObjectBook(const api::ObjectBook<maliput::math::Vector3>* object_book) { object_book_ = object_book; }
  /// @param road_network RoadNetwork to be returned when calling road_network() method.
  void SetRoadNetwork(const maliput::api::RoadNetwork* road_network) { road_network_ = road_network; }
  /// @param lanes_by_object Lanes to be returned when calling FindOverlappingLanesIn() method.
  void SetFindOverlappingLanesIn(const std::vector<const maliput::api::Lane*>& lanes_by_object) {
    lanes_by_object_ = lanes_by_object;
  }
  /// @param lanes_by_overlapping_type Lanes to be returned when calling FindOverlappingLanesIn() method with an
  /// overlapping type.
  void SetFindOverlappingLanesInByType(const std::vector<const maliput::api::Lane*>& lanes_by_overlapping_type) {
    lanes_by_overlapping_type_ = lanes_by_overlapping_type;
  }
  /// @param lane_s_route LaneSRoute to be returned when calling Route() method.
  void SetRoute(const maliput::api::LaneSRoute& lane_s_route) { lane_s_route_ = lane_s_route; }

 private:
  std::vector<const maliput::api::Lane*> DoFindOverlappingLanesIn(
      const api::Object<maliput::math::Vector3>* object) const override {
    return lanes_by_object_;
  }
  std::vector<const maliput::api::Lane*> DoFindOverlappingLanesIn(
      const api::Object<maliput::math::Vector3>* object, const api::OverlappingType& overlapping_type) const override {
    return lanes_by_overlapping_type_;
  }
  std::optional<const maliput::api::LaneSRoute> DoRoute(
      const api::Object<maliput::math::Vector3>* origin,
      const api::Object<maliput::math::Vector3>* target) const override {
    return std::make_optional<>(lane_s_route_);
  }
  const api::ObjectBook<maliput::math::Vector3>* do_object_book() const override { return object_book_; }
  const maliput::api::RoadNetwork* do_road_network() const override { return road_network_; }

  const api::ObjectBook<maliput::math::Vector3>* object_book_;
  const maliput::api::RoadNetwork* road_network_;
  std::vector<const maliput::api::Lane*> lanes_by_object_;
  std::vector<const maliput::api::Lane*> lanes_by_overlapping_type_;
  maliput::api::LaneSRoute lane_s_route_;
};

}  // namespace test_utilities
}  // namespace object
}  // namespace maliput
