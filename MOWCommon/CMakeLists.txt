project(MOWCommon)
add_library(MOWCommon
  MOWStringConverter.cpp
  MOWTimer.cpp
  MOWVector.cpp
  MOWInput.cpp
  MOWTransform.cpp

  MOWStringConverter.h
  MOWTimer.h
  MOWVector.h
  MOWInput.h
  MOWTransform.h
)

include_directories("${BASE_DIR}")