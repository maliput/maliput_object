ament_add_gtest(bounding_region_test bounding_region_test.cc)
ament_add_gtest(object_test object_test.cc)

macro(add_dependencies_to_test target)
    if (TARGET ${target})

      target_include_directories(${target}
        PRIVATE
          ${PROJECT_SOURCE_DIR}/include
          ${CMAKE_CURRENT_SOURCE_DIR}
          ${PROJECT_SOURCE_DIR}/test
      )

      target_link_libraries(${target}
          maliput::common
          maliput::math
          maliput_object::api
      )

    endif()
endmacro()

add_dependencies_to_test(bounding_region_test)
add_dependencies_to_test(object_test)
