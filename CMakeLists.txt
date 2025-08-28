cmake_minimum_required(VERSION 3.15)
project(se-proj)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Pull GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.9   # stable release
)
FetchContent_MakeAvailable(glfw)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/glad.c
)

target_include_directories(${PROJECT_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/include
)

find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME}
  PRIVATE glfw
  PRIVATE OpenGL::GL
)
