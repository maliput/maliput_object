// Copyright 2022 Toyota Research Institute
#include "maliput_object/loader/loader.h"

#include <map>
#include <string>
#include <utility>

#include <maliput/common/maliput_throw.h>
#include <yaml-cpp/yaml.h>

#include "maliput_object/api/object.h"
#include "maliput_object/base/bounding_box.h"
#include "maliput_object/base/manual_object_book.h"

namespace YAML {

template <>
struct convert<maliput::math::Vector3> {
  static Node encode(const maliput::math::Vector3& rhs) {
    Node node;
    node.push_back(rhs.x());
    node.push_back(rhs.y());
    node.push_back(rhs.z());
    return node;
  }

  static bool decode(const Node& node, maliput::math::Vector3& rhs) {
    MALIPUT_THROW_UNLESS(node.IsSequence());
    MALIPUT_THROW_UNLESS(node.size() == 3);
    rhs.x() = node[0].as<double>();
    rhs.y() = node[1].as<double>();
    rhs.z() = node[2].as<double>();
    return true;
  }
};

template <>
struct convert<maliput::math::RollPitchYaw> {
  static Node encode(const maliput::math::RollPitchYaw& rhs) {
    Node node;
    node.push_back(rhs.roll_angle());
    node.push_back(rhs.pitch_angle());
    node.push_back(rhs.yaw_angle());
    return node;
  }

  static bool decode(const Node& node, maliput::math::RollPitchYaw& rhs) {
    MALIPUT_THROW_UNLESS(node.IsSequence());
    MALIPUT_THROW_UNLESS(node.size() == 3);
    rhs.set(node[0].as<double>(), node[1].as<double>(), node[2].as<double>());
    return true;
  }
};

}  // namespace YAML

namespace maliput {
namespace object {
namespace loader {
namespace {
// TODO(#15): Decide to pass it as a construction argument or read it from the input file.
constexpr const double kTolerance{1e-3};

std::unique_ptr<BoundingBox> ParseBoundingBox(const YAML::Node& node, double tolerance) {
  MALIPUT_THROW_UNLESS(node.IsMap());
  MALIPUT_THROW_UNLESS(node["position"].IsDefined());
  MALIPUT_THROW_UNLESS(node["rotation"].IsDefined());
  MALIPUT_THROW_UNLESS(node["type"].IsDefined() && node["type"].as<std::string>() == "box");
  MALIPUT_THROW_UNLESS(node["box_size"].IsDefined());

  const maliput::math::Vector3 position = node["position"].as<maliput::math::Vector3>();
  const maliput::math::RollPitchYaw& rotation = node["rotation"].as<maliput::math::RollPitchYaw>();
  const maliput::math::Vector3 box_size = node["box_size"].as<maliput::math::Vector3>();

  return std::make_unique<BoundingBox>(position, box_size, rotation, tolerance);
}

std::map<std::string, std::string> ParseProperties(const YAML::Node& node) {
  MALIPUT_THROW_UNLESS(node.IsMap());
  std::map<std::string, std::string> properties;

  for (YAML::const_iterator property_it = node.begin(); property_it != node.end(); ++property_it) {
    properties.emplace(property_it->first.as<std::string>(), property_it->second.as<std::string>());
  }

  return properties;
}

std::unique_ptr<api::Object<maliput::math::Vector3>> ParseObject(const std::string& id, const YAML::Node& node,
                                                                 double tolerance) {
  MALIPUT_THROW_UNLESS(node["bounding_region"].IsDefined());
  MALIPUT_THROW_UNLESS(node["properties"].IsDefined());
  std::unique_ptr<BoundingBox> bounding_box = ParseBoundingBox(node["bounding_region"], tolerance);
  const std::map<std::string, std::string> properties = ParseProperties(node["properties"]);
  return std::make_unique<api::Object<maliput::math::Vector3>>(api::Object<maliput::math::Vector3>::Id(id), properties,
                                                               std::move(bounding_box));
}

std::unique_ptr<maliput::object::api::ObjectBook<maliput::math::Vector3>> BuildFrom(const YAML::Node& node) {
  MALIPUT_THROW_UNLESS(node.IsMap());
  const YAML::Node& objects_node = node["maliput_objects"];
  MALIPUT_THROW_UNLESS(objects_node.IsDefined());
  MALIPUT_THROW_UNLESS(objects_node.IsMap());
  auto object_book = std::make_unique<ManualObjectBook<maliput::math::Vector3>>();
  for (const auto& object_node : objects_node) {
    object_book->AddObject(ParseObject(object_node.first.as<std::string>(), object_node.second, kTolerance));
  }
  return object_book;
}

}  // namespace

std::unique_ptr<maliput::object::api::ObjectBook<maliput::math::Vector3>> Load(const std::string& input) {
  return BuildFrom(YAML::Load(input));
}

std::unique_ptr<maliput::object::api::ObjectBook<maliput::math::Vector3>> LoadFile(const std::string& filename) {
  return BuildFrom(YAML::LoadFile(filename));
}

}  // namespace loader
}  // namespace object
}  // namespace maliput
