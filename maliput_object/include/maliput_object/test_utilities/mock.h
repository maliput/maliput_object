// Copyright 2022 Toyota Research Institute
#include <memory>
#include <unordered_map>

#include <gtest/gtest.h>
#include <maliput/math/vector.h>

#include "maliput_object/api/bounding_region.h"
#include "maliput_object/api/object.h"
#include "maliput_object/api/object_book.h"

namespace maliput {
namespace object {
namespace test_utilities {

/// Mocks BoundingRegion class hardcoding the api's return values.
class MockBoundingRegion : public api::BoundingRegion<maliput::math::Vector3> {
 public:
  MockBoundingRegion() = default;
  MockBoundingRegion(const maliput::math::Vector3& position, bool contains,
                     const api::BoundingRegion<maliput::math::Vector3>::OverlappingType& overlapping_type)
      : position_(position), contains_(contains), overlapping_type_(overlapping_type) {}
  const maliput::math::Vector3& do_position() const override { return position_; }
  bool DoContains(const maliput::math::Vector3& position) const override { return contains_; }
  OverlappingType DoOverlaps(const BoundingRegion<maliput::math::Vector3>& other) const override {
    return overlapping_type_;
  }

  void SetPosition(const maliput::math::Vector3& position) { position_ = position; }
  void SetContains(bool contains) { contains_ = contains; }
  void SetOverlappingType(const OverlappingType& overlapping_type) { overlapping_type_ = overlapping_type; }

 private:
  maliput::math::Vector3 position_{0., 0., 0.};
  bool contains_{false};
  OverlappingType overlapping_type_{OverlappingType::kDisjointed};
};

/// Mocks ObjectBook class hardcoding the api's return values.
class MockObjectBook : public api::ObjectBook<maliput::math::Vector3> {
 public:
  MockObjectBook() = default;

  /// Constructs a MockObjectBook.
  /// @param objects Objects to be returned when calling Objects() method.
  /// @param object_by_id Objects to be returned when calling FindBy() method and passing an Id.
  /// @param objects_by_predicate Objects to be returned when calling FindBy() method and passing a predicate.
  /// @param objects_by_overlapping Objects to be returned when calling FindOverlappingIn() method.
  MockObjectBook(
      const std::unordered_map<api::Object<maliput::math::Vector3>::Id, api::Object<maliput::math::Vector3>*>& objects,
      api::Object<maliput::math::Vector3>* object_by_id,
      const std::vector<api::Object<maliput::math::Vector3>*>& objects_by_predicate,
      const std::vector<api::Object<maliput::math::Vector3>*>& objects_by_overlapping)
      : objects_(objects),
        object_by_id_(object_by_id),
        objects_by_predicate_(objects_by_predicate),
        objects_by_overlapping_(objects_by_overlapping) {}

  std::unordered_map<api::Object<maliput::math::Vector3>::Id, api::Object<maliput::math::Vector3>*> do_objects()
      const override {
    return objects_;
  }
  api::Object<maliput::math::Vector3>* DoFindBy(const api::Object<maliput::math::Vector3>::Id&) const override {
    return object_by_id_;
  }
  std::vector<api::Object<maliput::math::Vector3>*> DoFindBy(
      std::function<bool(const api::Object<maliput::math::Vector3>*)>) const override {
    return objects_by_predicate_;
  }
  std::vector<api::Object<maliput::math::Vector3>*> DoFindOverlappingIn(
      const api::BoundingRegion<maliput::math::Vector3>&,
      const std::optional<api::BoundingRegion<maliput::math::Vector3>::OverlappingType>&) const override {
    return objects_by_overlapping_;
  }

  /// @param objects Objects to be returned when calling Objects() method.
  void SetObjects(const std::unordered_map<api::Object<maliput::math::Vector3>::Id,
                                           api::Object<maliput::math::Vector3>*>& objects) {
    objects_ = objects;
  }
  /// @param objects Objects to be returned when calling FindBy() method and passing an Id.
  void SetFindById(api::Object<maliput::math::Vector3>* objects) { object_by_id_ = objects; }
  /// @param objects Objects to be returned when calling FindBy() method and passing a predicate.
  void SetFindByPredicate(const std::vector<api::Object<maliput::math::Vector3>*>& objects) {
    objects_by_predicate_ = objects;
  }
  /// @param objects Objects to be returned when calling FindOverlappingIn() method.
  void SetFindByOverlapping(const std::vector<api::Object<maliput::math::Vector3>*>& objects) {
    objects_by_overlapping_ = objects;
  }

 private:
  std::unordered_map<api::Object<maliput::math::Vector3>::Id, api::Object<maliput::math::Vector3>*> objects_;
  api::Object<maliput::math::Vector3>* object_by_id_;
  std::vector<api::Object<maliput::math::Vector3>*> objects_by_predicate_;
  std::vector<api::Object<maliput::math::Vector3>*> objects_by_overlapping_;
};

}  // namespace test_utilities
}  // namespace object
}  // namespace maliput
