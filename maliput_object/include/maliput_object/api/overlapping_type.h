#pragma once

namespace maliput {
namespace object {
namespace api {

/// Holds the possible overlapping types between Objects' region.
///  - Example of use:
/// @code {.cpp}
/// void MyMethod(OverlappingType type);
/// ...
/// MyMethod(OverlappingType::kContained | OverlappingType::kIntersected);
/// @endcode
///  - Example of use:
/// @code {.cpp}
/// OverlappingType MyMethod();
/// ...
/// if(OverlappingType::kContained & MyMethod() == OverlappingType::kContained) {
///  // Do something.
/// }
/// @endcode
enum class OverlappingType : unsigned int {
  kDisjointed = 0,                   ///< No overlapping between bounding regions
  kIntersected = 1 << 0,             ///< Bounding regions intersects.
  kContained = 1 << 1,               ///< Entire bounding region is contained within another.
  kAll = kIntersected | kContained,  ///< All overlapping types.
};

// Union operator.
OverlappingType operator|(const OverlappingType& first, const OverlappingType& second);

// Intersection operator.
OverlappingType operator&(const OverlappingType& first, const OverlappingType& second);

}  // namespace api
}  // namespace object
}  // namespace maliput
