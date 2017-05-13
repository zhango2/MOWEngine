project(MOWPhysics)
add_library(MOWPhysics 
  MOWPhysics.cpp

  MOWPhysics.h
  IMOWPhysicalEntity.h
  IMOWPhysicsWorld.h
)

find_package(protobuf REQUIRED)

include_directories(${PROTOBUF_INCLUDE_DIRS})

include_directories("${BASE_DIR}")