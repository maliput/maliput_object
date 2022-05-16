// Copyright 2022 Toyota Research Institute
#include "maliput_object/loader/loader.h"

#include <maliput/common/maliput_throw.h>

namespace maliput {
namespace object {
namespace loader {

std::unique_ptr<maliput::object::api::ObjectBook<maliput::math::Vector3>> Load(const std::string&) {
  MALIPUT_THROW_MESSAGE("Unimplemented");
}

std::unique_ptr<maliput::object::api::ObjectBook<maliput::math::Vector3>> LoadFile(const std::string&) {
  MALIPUT_THROW_MESSAGE("Unimplemented");
}

}  // namespace loader
}  // namespace object
}  // namespace maliput
