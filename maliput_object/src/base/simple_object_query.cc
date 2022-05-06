// Copyright 2022 Toyota Research Institute
#include "maliput_object/base/simple_object_query.h"

#include <algorithm>

#include <maliput/common/maliput_throw.h>
#include <maliput/routing/derive_lane_s_routes.h>

#include "maliput_object/base/bounding_box.h"

namespace maliput {
namespace object {

SimpleObjectQuery::SimpleObjectQuery(const maliput::api::RoadNetwork* road_network,
                                     const api::ObjectBook<maliput::math::Vector3>* object_book)
    : road_network_(road_network), object_book_(object_book) {
  MALIPUT_THROW_UNLESS(road_network_ != nullptr);
  MALIPUT_THROW_UNLESS(object_book != nullptr);
}

std::vector<const maliput::api::Lane*> SimpleObjectQuery::DoFindOverlappingLanesIn(
    const api::Object<maliput::math::Vector3>* object) const {
  MALIPUT_THROW_UNLESS(object != nullptr);
  return DoFindOverlappingLanesIn(object, api::OverlappingType::kIntersected);
}
std::vector<const maliput::api::Lane*> SimpleObjectQuery::DoFindOverlappingLanesIn(
    const api::Object<maliput::math::Vector3>* object, const api::OverlappingType& overlapping_type) const {
  MALIPUT_THROW_UNLESS(object != nullptr);
  std::vector<const maliput::api::Lane*> overlapping_lanes;

  // TODO(francocipollone): BoundingRegion should provide a method to obtain the vertices of the bounding region to
  // avoid casting.
  const auto bb = static_cast<const BoundingBox&>(object->bounding_region());
  const auto vertices = bb.get_vertices();

  for (const auto& vertex : vertices) {
    // FindRoadPosition at each vertex of the bounding box using a radius large enough to include the center of the
    // bounding box.
    const double radius = (object->position() - vertex).norm();
    const std::vector<maliput::api::RoadPositionResult> road_position_results =
        road_network_->road_geometry()->FindRoadPositions(maliput::api::InertialPosition::FromXyz(vertex), radius);
    // The RoadPositionResults contain the closest points to the lanes contained in the sphere,
    // There could be lanes that even though overlapp with the object, their closest point to the vertex is outside the
    // bounding region, leading to not tracking those lanes. Therefore, once the lanes located in the sphere are
    // obtained, they are queried to obtain the closest RoadPosition to the center of bounding region.
    for (const auto& road_position_result : road_position_results) {
      // Lane could have been already added by other road_position_result.
      if (std::find(overlapping_lanes.begin(), overlapping_lanes.end(), road_position_result.road_position.lane) !=
          overlapping_lanes.end()) {
        continue;
      }
      const maliput::api::Lane* lane = road_position_result.road_position.lane;
      const maliput::api::LanePositionResult closest_lane_position_result =
          lane->ToLanePosition(maliput::api::InertialPosition::FromXyz(object->position()));
      // The lane position result will contain lane position that could lay outside the lane boundary(always within the
      // segment bounds). Therefore, the lane position is compared with the lane bounds of the lane. In case the lane
      // position is outside the lane bounds, the lane position and nearest position are recomputed with the boundary
      // r-coordinate.
      maliput::api::LanePosition lane_pos = closest_lane_position_result.lane_position;
      maliput::api::InertialPosition neareast_pos = closest_lane_position_result.nearest_position;
      const auto lane_bounds = lane->lane_bounds(lane_pos.s());
      if (lane_pos.r() > lane_bounds.max() || lane_pos.r() < lane_bounds.min()) {
        lane_pos.set_r(std::clamp(lane_pos.r(), lane_bounds.min(), lane_bounds.max()));
        neareast_pos = lane->ToInertialPosition(lane_pos);
      }

      // Check if the lane's closest point is within the bounding region.
      if (object->bounding_region().Contains(neareast_pos.xyz())) {
        overlapping_lanes.push_back(lane);
      }
    }
  }

  switch (overlapping_type) {
    case api::OverlappingType::kIntersected: {
      return overlapping_lanes;
      break;
    }
    case api::OverlappingType::kDisjointed: {
      const auto lanes = road_network_->road_geometry()->ById().GetLanes();
      std::vector<const maliput::api::Lane*> disjointed_lanes;
      disjointed_lanes.reserve(lanes.size() - overlapping_lanes.size());
      std::for_each(lanes.begin(), lanes.end(),
                    [&disjointed_lanes, &overlapping_lanes](
                        const std::pair<maliput::api::LaneId, const maliput::api::Lane*>& lane_id_lane) {
                      if (overlapping_lanes.end() == std::find_if(overlapping_lanes.begin(), overlapping_lanes.end(),
                                                                  [&lane_id_lane](const maliput::api::Lane* lane) {
                                                                    return lane->id() == lane_id_lane.first;
                                                                  })) {
                        disjointed_lanes.push_back(lane_id_lane.second);
                      }
                    });
      return disjointed_lanes;
      break;
    }
    case api::OverlappingType::kContained: {
      // TODO(#22): Implement kContained case.
      MALIPUT_THROW_MESSAGE("Not implemented yet for kContained overlapping type.");
      break;
    }
    default:
      MALIPUT_THROW_MESSAGE("Not a valid overlapping type.");
  }
}

std::optional<const maliput::api::LaneSRoute> SimpleObjectQuery::DoRoute(
    const api::Object<maliput::math::Vector3>* origin, const api::Object<maliput::math::Vector3>* target) const {
  const auto origin_road_pos_result =
      road_network_->road_geometry()->ToRoadPosition(maliput::api::InertialPosition::FromXyz(origin->position()));
  const auto target_road_pos_result =
      road_network_->road_geometry()->ToRoadPosition(maliput::api::InertialPosition::FromXyz(target->position()));
  const auto lane_s_route =
      maliput::routing::DeriveLaneSRoutes(origin_road_pos_result.road_position, target_road_pos_result.road_position,
                                          std::numeric_limits<double>::infinity());
  if (lane_s_route.empty()) {
    return std::nullopt;
  }
  const auto min_route =
      std::min_element(lane_s_route.begin(), lane_s_route.end(),
                       [](const maliput::api::LaneSRoute& left, const maliput::api::LaneSRoute& right) {
                         return left.length() < right.length();
                       });
  return std::make_optional(*min_route);
}
const api::ObjectBook<maliput::math::Vector3>* SimpleObjectQuery::do_object_book() const { return object_book_; }
const maliput::api::RoadNetwork* SimpleObjectQuery::do_road_network() const { return {road_network_}; }

}  // namespace object
}  // namespace maliput
