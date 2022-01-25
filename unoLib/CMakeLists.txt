add_library(unoLib src/GameLib.cpp src/Card.cpp)
 
message("GameLib current source dir = ${CMAKE_CURRENT_SOURCE_DIR}")
 
target_include_directories( unoLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)