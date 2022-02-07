add_library(unoLib src/GameLib.cpp src/Card.cpp "include/uno/Hand.h" "src/Hand.cpp" "include/uno/Game.h" "src/Game.cpp" "include/olcPixelGameEngine.h" "include/uno/Example.h")
 
message("GameLib current source dir = ${CMAKE_CURRENT_SOURCE_DIR}")
 
target_include_directories( unoLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)