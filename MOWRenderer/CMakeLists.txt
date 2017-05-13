project(MOWRenderer)
add_library(MOWRenderer
  MOWFrustum.cpp
  MOWRenderer.cpp
  MOWRenderToTexture.cpp

  MOWFrustum.h
  MOWRenderer.h
  MOWRenderToTexture.h
)

find_package(protobuf REQUIRED)

include_directories(${PROTOBUF_INCLUDE_DIRS})

include_directories("${BASE_DIR}")
include_directories("${EXTERNAL_DIR}")
