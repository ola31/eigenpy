#
# Copyright (c) 2014-2023 CNRS INRIA
#

# --- LIBRARY --- #
set(PYWRAP ${PROJECT_NAME}_pywrap)
set(PYWRAP
    ${PYWRAP}
    PARENT_SCOPE)

make_directory("${${PROJECT_NAME}_BINARY_DIR}/python/${PROJECT_NAME}")
include(${JRL_CMAKE_MODULES}/python-helpers.cmake)
include("${JRL_CMAKE_MODULES}/stubs.cmake")

add_custom_target(python)
set_target_properties(python PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD True)

add_library(${PYWRAP} SHARED main.cpp)
add_dependencies(python ${PYWRAP})
target_link_libraries(${PYWRAP} PUBLIC ${PROJECT_NAME})
# BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS spews conversion warnings from int to
# long unsigned int. Unfortunately, using literals does not work in a macro. As
# such, this turns them off for the entire wrapper:
if(NOT WIN32)
  target_compile_options(${PYWRAP} PRIVATE "-Wno-conversion")
endif()
if(IS_ABSOLUTE ${PYTHON_SITELIB})
  set(ABSOLUTE_PYTHON_SITELIB ${PYTHON_SITELIB})
else()
  set(ABSOLUTE_PYTHON_SITELIB ${CMAKE_INSTALL_PREFIX}/${PYTHON_SITELIB})
endif()
set(${PYWRAP}_INSTALL_DIR ${ABSOLUTE_PYTHON_SITELIB}/${PROJECT_NAME})

set_target_properties(
  ${PYWRAP}
  PROPERTIES PREFIX ""
             SUFFIX ${PYTHON_EXT_SUFFIX}
             LIBRARY_OUTPUT_DIRECTORY
             "${CMAKE_BINARY_DIR}/python/${PROJECT_NAME}"
             LIBRARY_OUTPUT_DIRECTORY_<CONFIG>
             "${CMAKE_BINARY_DIR}/python/${PROJECT_NAME}"
             RUNTIME_OUTPUT_DIRECTORY
             "${CMAKE_BINARY_DIR}/python/${PROJECT_NAME}"
             RUNTIME_OUTPUT_DIRECTORY_<CONFIG>
             "${CMAKE_BINARY_DIR}/python/${PROJECT_NAME}")

if(UNIX)
  get_relative_rpath(${${PYWRAP}_INSTALL_DIR} ${PYWRAP}_INSTALL_RPATH)
  set_target_properties(${PYWRAP} PROPERTIES INSTALL_RPATH
                                             "${${PYWRAP}_INSTALL_RPATH}")
endif()

install(TARGETS ${PYWRAP} DESTINATION ${${PYWRAP}_INSTALL_DIR})

# --- GENERATE STUBS
if(GENERATE_PYTHON_STUBS)
  load_stubgen()

  generate_stubs(${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_NAME}
                 ${ABSOLUTE_PYTHON_SITELIB} ${PYWRAP})
endif(GENERATE_PYTHON_STUBS)

# --- INSTALL SCRIPTS
set(PYTHON_FILES __init__.py)

foreach(python ${PYTHON_FILES})
  python_build(${PROJECT_NAME} ${python})
  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/eigenpy/${python}"
          DESTINATION ${${PYWRAP}_INSTALL_DIR})
endforeach(python)
