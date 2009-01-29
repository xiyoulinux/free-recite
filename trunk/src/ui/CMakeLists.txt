# Make sure the compiler can find include files from our ui library.
INCLUDE_DIRECTORIES(${FREERECITE_SOURCE_DIR}/src/core)

LINK_DIRECTORIES(${FREERECITE_BINARY_DIR}/src/core)

ADD_LIBRARY(FreeReciteUi SHARED Cui.cpp)

INSTALL(TARGETS FreeReciteUi LIBRARY DESTINATION lib)
