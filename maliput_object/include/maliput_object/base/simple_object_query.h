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
