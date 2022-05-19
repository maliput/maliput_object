# TODO(francocipollone): Improve this check by wrapping ament_add_pytest_test() function.
# When sanitizers are activated python scripts are disabled.
if (NOT ${SANITIZERS})
  ament_add_pytest_test(api_pytest
    api_test.py
    # Avoid pytest from importing the module stub
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()
