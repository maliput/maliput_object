// Copyright 2022 Toyota Research Institute
#include "maliput_object/api/object.h"

#include <maliput/math/vector.h>

namespace maliput {
namespace object {
namespace api {

template <typename Coordinate>
Object<Coordinate>::Object(const Id& id, const std::map<std::string, std::string>& properties,
                           std::unique_ptr<BoundingRegion<Coordinate>> region)
    : id_(id), properties_(properties), region_(std::move(region)) {}

template <typename Coordinate>
typename Object<Coordinate>::Id Object<Coordinate>::id() const {
  return id_;
}

template <typename Coordinate>
const BoundingRegion<Coordinate>& Object<Coordinate>::bounding_region() const {
  return *region_.get();
}

template <typename Coordinate>
const Coordinate& Object<Coordinate>::position() const {
  return region_->position();
};

template <typename Coordinate>
std::optional<std::string> Object<Coordinate>::get_property(const std::string& key) const {
  const auto value = properties_.find(key);
  return value != properties_.end() ? std::make_optional(value->second) : std::nullopt;
}

template <typename Coordinate>
const std::map<std::string, std::string>& Object<Coordinate>::get_properties() const {
  return properties_;
}

template class Object<maliput::math::Vector3>;

}  // namespace api
}  // namespace object
}  // namespace maliput
