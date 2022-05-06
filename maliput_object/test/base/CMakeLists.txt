ament_add_gmock(manual_object_book_test manual_object_book_test.cc)
ament_add_gtest(bounding_box_test bounding_box_test.cc)

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
        maliput_object::api
        maliput_object::base
      )

    endif()
endmacro()

add_dependencies_to_test(bounding_box_test)
add_dependencies_to_test(manual_object_book_test)
