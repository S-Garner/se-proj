cmake_minimum_required(VERSION 3.5)
project(se-proj) # This will create the ${PROJECT_NAME}

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Download SFML
FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.5.1
)

FetchContent_MakeAvailable(SFML)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/glad.c)

# target_include_dir tells CMake which dir to pass to the compiler
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link SFML modules
target_link_libraries(${PROJECT_NAME}
  PRIVATE
  sfml-graphics
  sfml-window
  sfml-system)
