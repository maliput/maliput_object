// Copyright 2022 Toyota Research Institute
#include "maliput_object/api/overlapping_type.h"

namespace maliput {
namespace object {
namespace api {

OverlappingType operator|(const OverlappingType& first, const OverlappingType& second) {
  return OverlappingType(static_cast<unsigned int>(first) | static_cast<unsigned int>(second));
}
OverlappingType operator&(const OverlappingType& first, const OverlappingType& second) {
  return OverlappingType(static_cast<unsigned int>(first) & static_cast<unsigned int>(second));
}

}  // namespace api
}  // namespace object
}  // namespace maliput
