// Copyright 2022 Toyota Research Institute
#pragma once

#include <optional>
#include <vector>

#include <maliput/api/road_network.h>
#include <maliput/common/maliput_copyable.h>
#include <maliput/math/vector.h>

#include "maliput_object/api/object.h"
#include "maliput_object/api/object_book.h"
#include "maliput_object/api/object_query.h"
#include "maliput_object/api/overlapping_type.h"

namespace maliput {
namespace object {

/// api::ObjectQuery Implementation.
/// The implementation uses maliput's api for finding the lanes.
/// Methods like ToRoadPosition or FindRoadPositions are extensively used.
class SimpleObjectQuery : public api::ObjectQuery {
 public:
  MALIPUT_DEFAULT_COPY_AND_MOVE_AND_ASSIGN(SimpleObjectQuery)
  SimpleObjectQuery(const maliput::api::RoadNetwork* road_network,
                    const api::ObjectBook<maliput::math::Vector3>* object_book);
  ~SimpleObjectQuery() = default;

 private:
  std::vector<const maliput::api::Lane*> DoFindOverlappingLanesIn(
      const api::Object<maliput::math::Vector3>* object) const;
  std::vector<const maliput::api::Lane*> DoFindOverlappingLanesIn(const api::Object<maliput::math::Vector3>* object,
                                                                  const api::OverlappingType& overlapping_type) const;
  std::optional<const maliput::api::LaneSRoute> DoRoute(const api::Object<maliput::math::Vector3>* origin,
                                                        const api::Object<maliput::math::Vector3>* target) const;
  const api::ObjectBook<maliput::math::Vector3>* do_object_book() const;
  const maliput::api::RoadNetwork* do_road_network() const;

  const maliput::api::RoadNetwork* road_network_;
  const api::ObjectBook<maliput::math::Vector3>* object_book_;
};

}  // namespace object
}  // namespace maliput
