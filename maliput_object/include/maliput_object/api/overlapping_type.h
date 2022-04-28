#pragma once

namespace maliput {
namespace object {
namespace api {

/// Holds the possible overlapping types between Objects' region.
///
/// Given two sets `A` and `B` :
/// - `A` intersects `B` iff `A` and `B` have at least one point in common.
/// - `A` contains `B` iff `A` contains all the points of `B`.
///   (Note that `A` contains `B` iff `A` intersects `B` and `B` contains `A`.)
/// - `A` disjoints `B` iff `A` and `B` have no points in common.
///
///  - Example of use:
/// @code {.cpp}
/// OverlappingType MyMethod();
/// ...
/// if(OverlappingType::kIntersected & MyMethod() == OverlappingType::kIntersected) {
///  // Do something.
/// }
/// @endcode
enum class OverlappingType : unsigned int {
  kDisjointed = 0,   ///< No overlapping between bounding regions
  kIntersected = 1,  ///< Bounding regions are intersected.
  kContained = 3,    ///< Bounding regions are contained.
};

// Union operator.
OverlappingType operator|(const OverlappingType& first, const OverlappingType& second);

// Intersection operator.
OverlappingType operator&(const OverlappingType& first, const OverlappingType& second);

}  // namespace api
}  // namespace object
}  // namespace maliput
