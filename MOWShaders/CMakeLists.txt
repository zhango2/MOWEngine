project(MOWShaders)
add_library(MOWShaders
  MOWColorShader.cpp
  MOWDeferredShader.cpp
  MOWDepthShader.cpp
  MOWLightShader.cpp
  MOWShader.cpp
  MOWShadowShader.cpp
  MOWTextureShader.cpp

  MOWColorShader.h
  MOWDeferredShader.h
  MOWDepthShader.h
  MOWLightShader.h
  MOWShader.h
  MOWShadowShader.h
  MOWTextureShader.h
  ShaderTypes.h

  Color.fx
  Common.fx
  Deferred.fx
  Light.fx
  Shadow.fx
  Texture.fx

)

STRING(REGEX REPLACE "/" "\\\\" WIN_CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})

add_custom_command(
  TARGET MOWShaders
  PRE_BUILD
  COMMAND fxc /Od /Zi /T fx_5_0 ${WIN_CURRENT_DIR}\\Color.fx /Fo Color.fxo
  COMMAND fxc /Od /Zi /T fx_5_0 ${WIN_CURRENT_DIR}\\Common.fx /Fo Common.fxo
  COMMAND fxc /Od /Zi /T fx_5_0 ${WIN_CURRENT_DIR}\\Deferred.fx /Fo Deferred.fxo
  COMMAND fxc /Od /Zi /T fx_5_0 ${WIN_CURRENT_DIR}\\Light.fx /Fo Light.fxo
  COMMAND fxc /Od /Zi /T fx_5_0 ${WIN_CURRENT_DIR}\\Shadow.fx /Fo Shadow.fxo
  COMMAND fxc /Od /Zi /T fx_5_0 ${WIN_CURRENT_DIR}\\Texture.fx /Fo Texture.fxo
)

include_directories("${BASE_DIR}")
include_directories("${EXTERNAL_DIR}")

