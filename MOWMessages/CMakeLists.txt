project(MOWMessages)
add_definitions("/wd4996")
add_library(MOWMessages
  MOWCommon.pb.cc
  MOWGraphics.pb.cc

  MOWCommon.pb.h
  MOWGraphics.pb.h

  MOWCommon.proto
  MOWGraphics.proto
)

find_package(protobuf REQUIRED)

include_directories(${PROTOBUF_INCLUDE_DIRS})

include_directories("${BASE_DIR}")
include_directories("${EXTERNAL_DIR}")