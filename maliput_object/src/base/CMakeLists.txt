##############################################################################
# Sources
##############################################################################

set(BASE_SOURCES
  bounding_box.cc
  simple_object_query.cc
)

add_library(base ${BASE_SOURCES})

add_library(maliput_object::base ALIAS base)

set_target_properties(base
  PROPERTIES
    OUTPUT_NAME maliput_object_base
)

target_include_directories(base
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(base
  PUBLIC
  maliput::api
  maliput::common
  maliput::math
  maliput::routing
  maliput_object::api
)

##############################################################################
# Export
##############################################################################

include(CMakePackageConfigHelpers)

install(
  TARGETS base
  EXPORT ${PROJECT_NAME}-targets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

ament_export_libraries(base)
