##############################################################################
# Sources
##############################################################################

set(LOADER_SOURCES
  loader.cc
)

add_library(loader ${LOADER_SOURCES})

add_library(maliput_object::loader ALIAS loader)

set_target_properties(loader
  PROPERTIES
    OUTPUT_NAME maliput_object_loader
)

target_include_directories(loader
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(loader
  PUBLIC
  maliput::common
  maliput::math
  maliput_object::api
  maliput_object::base
  yaml-cpp
)

##############################################################################
# Export
##############################################################################

include(CMakePackageConfigHelpers)

install(
  TARGETS loader
  EXPORT ${PROJECT_NAME}-targets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
