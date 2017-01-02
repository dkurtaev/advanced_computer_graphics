cmake_minimum_required(VERSION 2.8)

project(advanced_computer_graphics)

find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${OpenGL_INCLUDE_DIRS}
  ${GLUT_INCLUDE_DIRS}
)

file(GLOB_RECURSE sources "src/*.cpp")
file(GLOB_RECURSE headers "include/*.hpp")

add_executable(${CMAKE_PROJECT_NAME} ${sources} ${headers})
target_link_libraries(${CMAKE_PROJECT_NAME}
  ${OPENGL_LIBRARIES}
  ${GLUT_LIBRARIES}
  ${OpenCV_LIBRARIES}
)
