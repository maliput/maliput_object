// BSD 3-Clause License
//
// Copyright (c) 2022, Woven Planet. All rights reserved.
// Copyright (c) 2022, Toyota Research Institute. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include <maliput/math/vector.h>
#include <maliput_object/api/bounding_region.h>
#include <maliput_object/api/object.h>
#include <maliput_object/api/object_book.h>
#include <maliput_object/api/object_query.h>
#include <maliput_object/api/overlapping_type.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace maliput {
namespace object {
namespace bindings {

namespace py = pybind11;

PYBIND11_MODULE(api, m) {
  py::class_<api::BoundingRegion<maliput::math::Vector3>>(m, "BoundingRegion")
      .def("position", &api::BoundingRegion<maliput::math::Vector3>::position,
           py::return_value_policy::reference_internal)
      .def("Contains", &api::BoundingRegion<maliput::math::Vector3>::Contains, py::arg("position"))
      .def("Overlaps", &api::BoundingRegion<maliput::math::Vector3>::Overlaps, py::arg("other"));

  py::class_<api::ObjectBook<maliput::math::Vector3>>(m, "ObjectBook")
      .def("objects", &api::ObjectBook<maliput::math::Vector3>::objects, py::return_value_policy::reference_internal)
      .def("FindById", &api::ObjectBook<maliput::math::Vector3>::FindById)
      .def("FindByPredicate", &api::ObjectBook<maliput::math::Vector3>::FindByPredicate)
      .def("FindOverlappingIn", &api::ObjectBook<maliput::math::Vector3>::FindOverlappingIn, py::arg("region"),
           py::arg("overlapping_type"));

  py::class_<api::ObjectQuery>(m, "ObjectQuery")
      .def("FindOverlappingLanesIn",
           py::overload_cast<const api::Object<maliput::math::Vector3>*>(&api::ObjectQuery::FindOverlappingLanesIn,
                                                                         py::const_),
           py::arg("object"))
      .def("FindOverlappingLanesIn",
           py::overload_cast<const api::Object<maliput::math::Vector3>*, const api::OverlappingType&>(
               &api::ObjectQuery::FindOverlappingLanesIn, py::const_),
           py::arg("object"), py::arg("overlapping_type"))
      .def("Route", &api::ObjectQuery::Route, py::arg("origin"), py::arg("target"))
      .def("object_book", &api::ObjectQuery::object_book, py::return_value_policy::reference_internal)
      .def("road_network", &api::ObjectQuery::road_network, py::return_value_policy::reference_internal);

  py::class_<api::Object<maliput::math::Vector3>::Id>(m, "Id")
      .def(py::init<std::string>())
      .def(py::detail::hash(py::self))
      .def("string", &api::Object<maliput::math::Vector3>::Id::string)
      .def("__eq__", &api::Object<maliput::math::Vector3>::Id::operator==)
      .def("__repr__", [](const api::Object<maliput::math::Vector3>::Id& id) { return id.string(); });

  py::class_<api::Object<maliput::math::Vector3>>(m, "Object")
      .def(py::init<const api::Object<maliput::math::Vector3>::Id&, const std::map<std::string, std::string>&,
                    std::unique_ptr<api::BoundingRegion<maliput::math::Vector3>>>(),
           py::arg("id"), py::arg("properties"), py::arg("region"))
      .def("id", &api::Object<maliput::math::Vector3>::id)
      .def("bounding_region", &api::Object<maliput::math::Vector3>::bounding_region,
           py::return_value_policy::reference_internal)
      .def("position", &api::Object<maliput::math::Vector3>::position, py::return_value_policy::reference_internal)
      .def("get_properties", &api::Object<maliput::math::Vector3>::get_properties,
           py::return_value_policy::reference_internal)
      .def("get_property", &api::Object<maliput::math::Vector3>::get_property, py::arg("key"));

  py::enum_<api::OverlappingType>(m, "OverlappingType", py::arithmetic())
      .value("kDisjointed", api::OverlappingType::kDisjointed)
      .value("kIntersected", api::OverlappingType::kIntersected)
      .value("kContained", api::OverlappingType::kContained)
      .def("__and__", api::operator&)
      .def("__or__", api::operator|);
}
}  // namespace bindings
}  // namespace object
}  // namespace maliput
