add_library(unoLib src/GameLib.cpp src/Card.cpp "include/uno/Hand.h" "src/Hand.cpp" "include/uno/Game.h" "src/Game.cpp" "src/glad.c")
 
message("GameLib current source dir = ${CMAKE_CURRENT_SOURCE_DIR}")
 
target_include_directories( unoLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(unoLib ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib ${OPENGL_gl_LIBRARY})