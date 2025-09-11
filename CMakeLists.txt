cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(DepressionEngine)

include(FetchContent)
FetchContent_Declare(
      glfw
      GIT_REPOSITORY https://github.com/glfw/glfw.git
      GIT_TAG          3.4
)
FetchContent_MakeAvailable(glfw)

add_subdirectory(src/vk_render)

add_executable(DepressionEngine "src/Engine.h" "src/Engine.cpp" "src/IRender.h")

target_link_libraries(DepressionEngine PUBLIC glfw)
target_link_libraries(DepressionEngine PUBLIC VulkanRender)
