// Copyright 2022 Toyota Research Institute
#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>

#include <maliput/api/type_specific_identifier.h>
#include <maliput/common/maliput_copyable.h>

#include "maliput_object/api/bounding_region.h"

namespace maliput {
namespace object {
namespace api {

/// Represents an object in a given @p Coordinate system.
template <typename Coordinate>
class Object {
 public:
  MALIPUT_NO_COPY_NO_MOVE_NO_ASSIGN(Object)
  using Id = maliput::api::TypeSpecificIdentifier<class Object>;

  /// Constructs an Object.
  /// @param id Id of the object.
  /// @param properties Object's properties.
  /// @param region Object's bounding region.
  /// @tparam Coordinate Coordinate of the bounding region instance.
  Object(const Id& id, const std::map<std::string, std::string>& properties,
         std::unique_ptr<BoundingRegion<Coordinate>> region);

  ~Object() = default;

  /// @returns The id of the Object.
  Id id() const;

  /// @returns The bounding region of the object.
  const BoundingRegion<Coordinate>& bounding_region() const;

  /// @returns The position of the object in the Inertial-.
  const Coordinate& position() const;

  /// @returns A property of the object.
  std::optional<std::string> get_property(const std::string& key) const;

  /// @returns All the properties of the object.
  const std::map<std::string, std::string>& get_properties() const;

 private:
  const Id id_;
  const std::map<std::string, std::string> properties_;
  const std::unique_ptr<BoundingRegion<Coordinate>> region_;
};

}  // namespace api
}  // namespace object
}  // namespace maliput
