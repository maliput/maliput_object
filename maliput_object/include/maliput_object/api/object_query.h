// Copyright 2022 Toyota Research Institute
#pragma once

#include <optional>
#include <vector>

#include <maliput/api/lane.h>
#include <maliput/api/road_network.h>
#include <maliput/common/maliput_copyable.h>
#include <maliput/math/vector.h>

#include "maliput_object/api/object.h"
#include "maliput_object/api/object_book.h"
#include "maliput_object/api/overlapping_type.h"

namespace maliput {
namespace object {
namespace api {

/// Interface for performing queries on top of Maliput's RoadNetwork.
/// To match convention of underlying RoadNetwork, the query interface use maliput::math::Vector3
/// as Coordinate template argument for defining the coordinate system.
class ObjectQuery {
 public:
  MALIPUT_DEFAULT_COPY_AND_MOVE_AND_ASSIGN(ObjectQuery)

  virtual ~ObjectQuery() = default;

  /// Finds all the lanes overlapping with @p object .
  /// @param object Object to find lanes overlapping with.
  /// @returns A vector of pointers to lanes overlapping with @p object .
  std::vector<const maliput::api::Lane*> FindOverlappingLanesIn(const Object<maliput::math::Vector3>* object) const {
    return DoFindOverlappingLanesIn(object);
  }

  /// Finds all the lanes overlapping according the @p overlapping_type with @p object .
  /// @param object Object to find lanes overlapping with.
  /// @param overlapping_type Type of overlapping to find.
  /// @returns A vector of pointers to lanes overlapping with @p object .
  std::vector<const maliput::api::Lane*> FindOverlappingLanesIn(const Object<maliput::math::Vector3>* object,
                                                                const OverlappingType& overlapping_type) const {
    return DoFindOverlappingLanesIn(object, overlapping_type);
  }

  /// Finds the route between @p origin and @p target objects.
  /// @param origin Object to find route from.
  /// @param target Object to find route to.
  std::optional<const maliput::api::LaneSRoute> Route(const Object<maliput::math::Vector3>* origin,
                                                      const Object<maliput::math::Vector3>* target) const {
    return DoRoute(origin, target);
  }

  /// @returns The ObjectBook.
  const ObjectBook<maliput::math::Vector3>* object_book() const { return do_object_book(); }
  /// @returns The maliput::api::RoadNetwork.
  const maliput::api::RoadNetwork* road_network() const { return do_road_network(); }

 protected:
  ObjectQuery() = default;

 private:
  virtual std::vector<const maliput::api::Lane*> DoFindOverlappingLanesIn(
      const Object<maliput::math::Vector3>* object) const = 0;
  virtual std::vector<const maliput::api::Lane*> DoFindOverlappingLanesIn(
      const Object<maliput::math::Vector3>* object, const OverlappingType& overlapping_type) const = 0;
  virtual std::optional<const maliput::api::LaneSRoute> DoRoute(const Object<maliput::math::Vector3>* origin,
                                                                const Object<maliput::math::Vector3>* target) const = 0;
  virtual const ObjectBook<maliput::math::Vector3>* do_object_book() const = 0;
  virtual const maliput::api::RoadNetwork* do_road_network() const = 0;
};

}  // namespace api
}  // namespace object
}  // namespace maliput
