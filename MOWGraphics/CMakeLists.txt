project(MOWGraphics)
add_library(MOWGraphics 
  MOW2DObject.cpp
  MOWAlignedObject.cpp
  MOWBoundingBox.cpp
  MOWBox.cpp
  MOWCamera.cpp
  MOWClass.cpp
  MOWFace.cpp
  MOWGraphics.cpp
  MOWLight.cpp
  MOWLightCreator.cpp
  MOWMaterial.cpp
  MOWModel.cpp
  MOWModelPart.cpp
  MOWObject.cpp
  MOWOctTreeNode.cpp
  MOWPointLight.cpp
  MOWRenderableObject.cpp
  MOWResourceManager.cpp
  MOWScene.cpp
  MOWSphere.cpp
  MOWTriangle.cpp
  MOWVariant.cpp
  MOWViewPoint.cpp
  MOWBone.cpp

  MOW2DObject.h
  MOWAlignedObject.h
  MOWBoundingBox.h
  MOWBox.h
  MOWCamera.h
  MOWClass.h
  MOWCommon.h
  MOWFace.h
  MOWGraphics.h
  MOWLight.h
  MOWLightCreator.h
  MOWMaterial.h
  MOWModel.h
  MOWModelPart.h
  MOWObject.h
  MOWOctTreeNode.h
  MOWPointLight.h
  MOWRenderableObject.h
  MOWResourceManager.h
  MOWScene.h
  MOWSphere.h
  MOWTriangle.h
  MOWVariant.h
  MOWViewPoint.h
  MOWBone.h
  MOWCommon.h
)

find_package(protobuf REQUIRED)

include_directories(${PROTOBUF_INCLUDE_DIRS})


include_directories("${BASE_DIR}")
include_directories("${EXTERNAL_DIR}")


