# Recurse into the "core" and "ui" subdirectories.
ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(ui)

# Make sure the compiler can find include files from our ui library.
INCLUDE_DIRECTORIES(${FREERECITE_SOURCE_DIR}/src/ui)

# Make sure the linker can find the ui library once it is built.
LINK_DIRECTORIES(${FREERECITE_BINARY_DIR}/src/ui)

# Make sure the compiler can find include files from our ui library.
INCLUDE_DIRECTORIES(${FREERECITE_SOURCE_DIR}/src/core)

# Make sure the linker can find the ui library once it is built.
LINK_DIRECTORIES(${FREERECITE_BINARY_DIR}/src/core)

MESSAGE(STATUS "This is BINARY dir " ${FREERECITE_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir " ${FREERECITE_SOURCE_DIR})

ADD_EXECUTABLE(frt main.cpp)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
TARGET_LINK_LIBRARIES(frt FreeReciteUi)
TARGET_LINK_LIBRARIES(frt FreeReciteCore)

INSTALL(PROGRAMS frt-init DESTINATION bin)

INSTALL(PROGRAMS ${PROJECT_BINARY_DIR}/bin/frt DESTINATION bin)
