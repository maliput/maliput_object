ament_add_gtest(loader_test loader_test.cc)

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
        maliput::test_utilities
        fmt::fmt
        maliput_object::api
        maliput_object::base
        maliput_object::loader
      )

    endif()
endmacro()

add_dependencies_to_test(loader_test)
