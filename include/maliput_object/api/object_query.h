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
#pragma once

#include <optional>
#include <vector>

#include <maliput/api/lane.h>
#include <maliput/api/road_network.h>
#include <maliput/common/maliput_copyable.h>
#include <maliput/math/overlapping_type.h>
#include <maliput/math/vector.h>

#include "maliput_object/api/object.h"
#include "maliput_object/api/object_book.h"

namespace maliput {
namespace object {
namespace api {

/// Interface to perform queries on top of Maliput's RoadNetwork about Objects.
/// To match convention of underlying RoadNetwork, the query interface use maliput::math::Vector3
/// as the specialization of the Coordinate template argument.
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
  std::vector<const maliput::api::Lane*> FindOverlappingLanesIn(
      const Object<maliput::math::Vector3>* object, const maliput::math::OverlappingType& overlapping_type) const {
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
      const Object<maliput::math::Vector3>* object, const maliput::math::OverlappingType& overlapping_type) const = 0;
  virtual std::optional<const maliput::api::LaneSRoute> DoRoute(const Object<maliput::math::Vector3>* origin,
                                                                const Object<maliput::math::Vector3>* target) const = 0;
  virtual const ObjectBook<maliput::math::Vector3>* do_object_book() const = 0;
  virtual const maliput::api::RoadNetwork* do_road_network() const = 0;
};

}  // namespace api
}  // namespace object
}  // namespace maliput
