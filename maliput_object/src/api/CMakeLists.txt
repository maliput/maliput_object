##############################################################################
# Sources
##############################################################################

set(API_SOURCES
  object.cc
  overlapping_type.cc
)

add_library(api ${API_SOURCES})

add_library(maliput_object::api ALIAS api)

set_target_properties(api
  PROPERTIES
    OUTPUT_NAME maliput_object_api
)

target_include_directories(api
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(api
  PUBLIC
  maliput::api
  maliput::common
)

##############################################################################
# Export
##############################################################################

include(CMakePackageConfigHelpers)

install(
  TARGETS api
  EXPORT ${PROJECT_NAME}-targets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

ament_export_libraries(api)
