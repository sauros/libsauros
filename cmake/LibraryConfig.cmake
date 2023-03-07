# Select library type
set(_PN ${PROJECT_NAME})
option(BUILD_SHARED_LIBS "Build ${_PN} as a shared library." ON)
if(BUILD_SHARED_LIBS)
  set(LIBRARY_TYPE SHARED)
else()
  set(LIBRARY_TYPE STATIC)
  message(WARNING "Static builds can not utilize sauros packages")
endif()

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release'.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)

  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Target
add_library(${LIBRARY_NAME} ${LIBRARY_TYPE} ${SOURCES} ${HEADERS})

# Install library
install(TARGETS ${LIBRARY_NAME}
  EXPORT ${PROJECT_EXPORT}
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT stlib
  COMPONENT dev)

# Create 'version.h'
configure_file(${PROJECT_SOURCE_DIR}/libsauros/generate/version.hpp.in
  "${PROJECT_SOURCE_DIR}/libsauros/version.hpp" @ONLY)
set(HEADERS ${HEADERS} ${PROJECT_SOURCE_DIR}/libsauros/version.hpp)

# Install headers
#install(FILES ${HEADERS}
#  DESTINATION "${INSTALL_INCLUDE_DIR}/${LIBRARY_FOLDER}" )
install(
    DIRECTORY
    "${PROJECT_SOURCE_DIR}/libsauros"
    DESTINATION
    "${INSTALL_INCLUDE_DIR}"
    FILES_MATCHING
    PATTERN
    "*.hpp"
)