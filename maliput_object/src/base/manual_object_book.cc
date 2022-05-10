// Copyright 2022 Toyota Research Institute
#include "maliput_object/base/manual_object_book.h"

#include <maliput/math/vector.h>

namespace maliput {
namespace object {

template <typename Coordinate>
void ManualObjectBook<Coordinate>::AddObject(std::unique_ptr<api::Object<Coordinate>> object) {
  MALIPUT_THROW_UNLESS(object != nullptr);
  objects_.emplace(object->id(), std::move(object));
}

template <typename Coordinate>
void ManualObjectBook<Coordinate>::RemoveObject(const typename api::Object<Coordinate>::Id& object) {
  MALIPUT_THROW_UNLESS(objects_.find(object) != objects_.end());
  objects_.erase(object);
}

template <typename Coordinate>
std::unordered_map<typename api::Object<Coordinate>::Id, api::Object<Coordinate>*>
ManualObjectBook<Coordinate>::do_objects() const {
  std::unordered_map<typename api::Object<Coordinate>::Id, api::Object<Coordinate>*> objects;
  objects.reserve(objects_.size());
  for (const auto& pair : objects_) {
    objects.emplace(pair.first, pair.second.get());
  }
  return objects;
}

template <typename Coordinate>
api::Object<Coordinate>* ManualObjectBook<Coordinate>::DoFindById(
    const typename api::Object<Coordinate>::Id& object_id) const {
  const auto it = objects_.find(object_id);
  return it == objects_.end() ? nullptr : it->second.get();
}

template <typename Coordinate>
std::vector<api::Object<Coordinate>*> ManualObjectBook<Coordinate>::DoFindByPredicate(
    std::function<bool(const api::Object<Coordinate>*)> predicate) const {
  std::vector<api::Object<Coordinate>*> result;
  std::for_each(objects_.begin(), objects_.end(), [&predicate, &result](const auto& pair) {
    if (predicate(pair.second.get())) {
      result.push_back(pair.second.get());
    }
  });
  return result;
}

template <typename Coordinate>
std::vector<api::Object<Coordinate>*> ManualObjectBook<Coordinate>::DoFindOverlappingIn(
    const api::BoundingRegion<Coordinate>& region, const api::OverlappingType& overlapping_type) const {
  std::vector<api::Object<Coordinate>*> result;
  std::for_each(objects_.begin(), objects_.end(), [&region, &overlapping_type, &result](const auto& pair) {
    if ((pair.second->bounding_region().Overlaps(region) & overlapping_type) == overlapping_type) {
      result.push_back(pair.second.get());
    }
  });
  return result;
}

template class ManualObjectBook<maliput::math::Vector3>;

}  // namespace object
}  // namespace maliput
