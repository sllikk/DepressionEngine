 

find_package(Vulkan REQUIRED)

if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan SDK not found. Please install Vulkan SDK and set VULKAN_SDK environment variable.")
else()
    message(STATUS "Vulkan SDK found!")
    message(STATUS "Include dirs: ${Vulkan_INCLUDE_DIRS}")
    message(STATUS "Libraries: ${Vulkan_LIBRARIES}")
endif()

set( render_h  vk_render.h) 
set( render_cpp  vk_render.cpp) 


add_library(VulkanRender STATIC ${render_h} ${render_cpp})
target_link_libraries(VulkanRender PUBLIC Vulkan::Vulkan)
target_link_libraries(VulkanRender PRIVATE glfw)