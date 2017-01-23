cmake_minimum_required(VERSION 3.6)
project(BallPit)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${BallPit_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
INCLUDE(FindSDL2 REQUIRED)
INCLUDE(FindOpenGL REQUIRED)
INCLUDE(FindGLEW REQUIRED)
INCLUDE(FindSDL2_ttf REQUIRED)

include_directories(${BallPit_SOURCE_DIR}/include
        ${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR})

set(SOURCE_FILES
        src/MainGame.cpp
        src/MainGame.h
        src/Ball.cpp
        src/Ball.h
        src/BallController.cpp
        src/BallController.h
        src/BallRenderer.cpp
        src/BallRenderer.h
        src/Grid.cpp
        src/Grid.h
        src/main.cpp)

add_executable(BallPit ${SOURCE_FILES})
include(FindPkgConfig)
find_package(libFalconLatinum REQUIRED)
include_directories(${FALCONLATINUM_INCLUDE_DIR})
target_link_libraries(BallPit
        ${SDL2_LIBRARY} ${OPENGL_LIBRARIES}
        ${FALCONLATINUM_LIBRARY} ${SDL2_TTF_LIBRARIES} ${GLEW_LIBRARIES})
