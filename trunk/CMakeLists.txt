# The name of our project is "FREERECITE". CMakeLists file in this project can
# refer to the root source directory of the project as ${FREERECITE_SOURCE_DIR}
# and to the root binary directory of the project as ${FREERECITE_BINARY_DIR}.

# Guarantee the version of CMake is 2.6 at least.
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(FREERECITE)

ADD_SUBDIRECTORY(dict)
ADD_SUBDIRECTORY(src)

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
