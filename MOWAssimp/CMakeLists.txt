project(MOWAssimp)
add_library(MOWAssimp
  MOWAssimp.cpp

  MOWAssimp.h
)

find_package(ASSIMP REQUIRED)
find_package(protobuf REQUIRED)

include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories("${ASSIMP_INCLUDE_DIR}")
include_directories("${BASE_DIR}")
include_directories("${EXTERNAL_DIR}")