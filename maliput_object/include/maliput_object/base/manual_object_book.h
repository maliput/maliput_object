// Copyright 2022 Toyota Research Institute
#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>

#include <maliput/common/maliput_copyable.h>

#include "maliput_object/api/bounding_region.h"
#include "maliput_object/api/object.h"
#include "maliput_object/api/object_book.h"
#include "maliput_object/api/overlapping_type.h"

namespace maliput {
namespace object {

/// Implements api::ObjectBook for loading objects manually.
template <typename Coordinate>
class ManualObjectBook : public api::ObjectBook<Coordinate> {
 public:
  MALIPUT_NO_COPY_NO_MOVE_NO_ASSIGN(ManualObjectBook)
  ManualObjectBook() = default;
  virtual ~ManualObjectBook() = default;

  /// Adds an object to the book.
  /// @param object The object to be added.
  void AddObject(std::unique_ptr<api::Object<Coordinate>> object);

  /// Removes an object from the book.
  /// @param object The object to be removed.
  void RemoveObject(const typename api::Object<Coordinate>::Id& object);

 private:
  virtual std::unordered_map<typename api::Object<Coordinate>::Id, api::Object<Coordinate>*> do_objects()
      const override;
  virtual api::Object<Coordinate>* DoFindById(const typename api::Object<Coordinate>::Id& object_id) const override;
  virtual std::vector<api::Object<Coordinate>*> DoFindByPredicate(
      std::function<bool(const api::Object<Coordinate>*)> predicate) const override;
  virtual std::vector<api::Object<Coordinate>*> DoFindOverlappingIn(
      const api::BoundingRegion<Coordinate>& region, const api::OverlappingType& overlapping_type) const override;

  std::unordered_map<typename api::Object<Coordinate>::Id, std::unique_ptr<api::Object<Coordinate>>> objects_;
};

}  // namespace object
}  // namespace maliput
