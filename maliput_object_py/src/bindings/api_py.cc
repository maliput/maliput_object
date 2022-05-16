#include <maliput_object/api/bounding_region.h>
#include <maliput_object/api/object_book.h>
#include <maliput_object/api/object_query.h>
#include <maliput_object/api/object.h>
#include <maliput_object/api/overlapping_type.h>
#include <maliput/math/vector.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace maliput{
namespace object{
namespace bindings{

namespace py = pybind11;

PYBIND11_MODULE(api, m) {

  py::class_<api::BoundingRegion>(m, "BoundingRegion")
      .def(py::init<>())
      .def("position", &api::BoundingRegion::position, py::return_value_policy::reference_internal)
      .def("Contains", &api::BoundingRegion::Contains, py::arg("position"))
      .def("Overlaps", &api::BoundingRegion::Overlaps, py::arg("other"));

  py::class_<api::ObjectBook>(m, "ObjectBook")
      .def(py::init<>())
      .def("objects", &api::ObjectBook::objects, py::return_value_policy::reference_internal)
      .def("FindById", &api::ObjectBook::FindById)
      .def("FindByPredicate", &api::ObjectBook::FindByPredicate)
      .def("FindOverlappingIn", &api::ObjectBook::FindOverlappingIn, py::arg("region"), py::arg("overlapping_type"));

  py::class_<api::ObjectQuery>(m, "ObjectQuery")
      .def(py::init<>())
      .def("FindOverlappingLanesIn", &api::ObjectQuery::FindOverlappingLanesIn, py::arg("object"))
      .def("FindOverlappingLanesIn", &api::ObjectQuery::FindOverlappingLanesIn, py::arg("object"), py::arg("overlapping_type"))
      .def("Route", &api::ObjectQuery::Route, py::arg("origin"), py::arg("target"))
      .def("object_book", &api::ObjectQuery::object_book, py::return_value_policy::reference_internal)
      .def("road_network", &api::ObjectQuery::road_network, py::return_value_policy::reference_internal);

  py::class_<api::Object::Id>(m, "Id")
      .def(py::init<std::string>())
      .def(py::detail::hash(py::self))
      .def("string", &api::Object::Id::string)
      .def("__eq__", &api::Object::Id::operator==)
      .def("__repr__", [](const api::Object::Id& id) { return id.string(); });

  py::class_<api::Object>(m, "Object")
      .def(py::init<const Object::Id&, const std::map<std::string, std::string>&, std::unique_ptr<BoundingRegion>,
       py::arg("id"), py::arg("properties"), py::arg("region"))
      .def("id", &api::Object::id, py::return_value_policy::reference_internal)
      .def("bounding_region", &api::Object::bounding_region, py::return_value_policy::reference_internal)
      .def("position", &api::Object::position, py::return_value_policy::reference_internal)
      .def("get_properties", &api::Object::get_properties, py::return_value_policy::reference_internal)
      .def("get_property", &api::Object::get_properties, py::arg("key"), py::return_value_policy::reference_internal);

  py::class_<api::OverlappingType>(m, "OverlappingType")
      .def("__or__", &api::Object::operator|)
      .def("__eq__", &api::Object::operator&);

}
}// namespace bindings
}// namespace object
}// namespace maliput
