// Copyright 2022 Toyota Research Institute
#pragma once

#include <memory>
#include <string>

#include <maliput/math/vector.h>

#include "maliput/object/api/object_book.h"

/// @section maliput_object_yaml YAML format specification for objects
///
/// The maliput::object API provides two functions to load a concrete
/// maliput::object::api::ObjectBook implementation from a YAML description. One
/// is thought for files ( @ref maliput::object::loader::LoadFile() "LoadFile()")
/// and the other for string serialized YAML descriptions ( @ref maliput::object::loader::Load() "Load()" ).
///
/// The only supported type of coordinate is maliput::math::Vector3 , meaning
/// that concrete @ref maliput::object::api::BoundingRegion "BoundingRegions"
/// are limited to @ref maliput::object::BoundingBox "BoundingBox".
///
/// @subsection maliput_object_yaml_format YAML format
///
/// See the following code snippet that shows an example of a YAML ObjectBook:
///
/// @code{.yml}
/// maliput_objects:
///   my_awesome_object:
///     pose:
///       position: [1.2, 3.4, 4.5]
///       rotation: [0.1745329, 0.3490659, 0.5235988]
///     bounding_region:
///       type: box
///       box_size: [1.1, 2.2, 3.3]
///     properties:
///       - user_defined_prop_1: my_value_1
///         ...
///         ...
///       - user_defined_prop_n: my_value_n
///   ...
/// @endcode
///
/// @subsubsection maliput_object_yaml_entities Entities
///
/// <h4>`maliput_objects`</h4>
///
/// `maliput_objects` is a dictionary of objects. Every first level key
/// identifies a new maliput::api::Object:
///
/// @code{.yml}
///
/// maliput_object:
///   object_1:
///      ...
///   object_2:
///      ...
///   object_n:
///      ...
///
/// @endcode
///
/// <h4>`pose`</h4>
///
/// The maliput::api::Object's `pose` is a mapping that contains two keys:
/// `position` and `rotation`.
///
/// @code{.yml}
///
/// pose:
///   position: [X, Y, Z]
///   rotation: [R, P, Y]
///
/// @endcode
///
/// Where:
///
/// - `position`: is the Inertial Frame position described as a 3D Vector: @f$[X, Y, Z]@f$.
/// - `rotation`: is the Inertial Frame rotation described as a 3D Vector of
///   Euler angles @f$[R, P, Y]@f$ (roll, pitch, yaw).
///
/// <h4>`bounding_region`</h4>
///
/// A mapping describing a bounding region. For boxes:
///
/// @code{.yml}
///
/// bounding_region:
///   type: box
///   box_size: [W, D, H]
///
/// @endcode
///
/// Where:
///
/// - `type`: must be `box`.
/// - `box_size`: is a 3D vector with the box size expressed as @f$[Width, Depth, Height]@f$.
///
/// <h4>`properties`</h4>
///
/// A listing of user defined properties. The listing is flat, meaning that it
/// is a collection of key-value pairs with no depth. Keys and values will be
/// read as strings.
///
/// @code{.yml}
///
/// properties:
///   - user_defined_prop_1: my_value_1
///   ...
///
///   - user_defined_prop_n: my_value_n
///
/// @endcode
///
/// Where:
///
/// - `user_defined_prop_1` and `user_defined_prop_n` are examples of keys.
/// - `my_value_1` and `my_value_n` are examples of values.

namespace maliput {
namespace object {
namespace loader {

/// Loads the @p input string as a `maliput_object` YAML document.
/// See @ref loader.h documentation for further details.
///
/// @param input A YAML document as a string that must contain a node as described in @ref loader.h
/// @throws maliput::common::assertion_error When @p input fails to be parsed.
/// @return A maliput::object::api::ObjectBook<maliput::math::Vector3> representing the @p input.
std::unique_ptr<maliput::object::api::ObjectBook<maliput::math::Vector3>> Load(const std::string& input);

/// Loads the @p filename file as a `maliput_object` YAML document.
/// See @ref loader.h documentation for further details.
///
/// @param filename The path to the YAML document.
/// @throws maliput::common::assertion_error When @p input fails to be parsed.
/// @return A maliput::object::api::ObjectBook<maliput::math::Vector3> representing the contents of @p filename.
std::unique_ptr<maliput::object::api::ObjectBook<maliput::math::Vector3>> LoadFile(const std::string& filename);

}  // namespace loader
}  // namespace object
}  // namespace maliput
