ament_add_gmock(object_book_test object_book_test.cc)
ament_add_gmock(object_test object_test.cc)
ament_add_gmock(object_query_test object_query_test.cc)

macro(add_dependencies_to_test target)
if (TARGET ${target})

      target_include_directories(${target}
        PRIVATE
          ${PROJECT_SOURCE_DIR}/include
          ${CMAKE_CURRENT_SOURCE_DIR}
          ${PROJECT_SOURCE_DIR}/test
      )

      target_link_libraries(${target}
          maliput::api
          maliput::common
          maliput::math
          maliput::test_utilities
          maliput_object::api
      )

    endif()
endmacro()

add_dependencies_to_test(object_book_test)
add_dependencies_to_test(object_query_test)
add_dependencies_to_test(object_test)
