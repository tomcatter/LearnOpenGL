# 构建该项目所需的CMake最低版本
cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

# 设置项目的名称
project(LearnOpenGL)
# 设置c++的标准版本
set(CMAKE_CXX_STANDARD 17)
# 控制是否强制执行CMAKE_CXX_STANDARD指定的标准，如果编译器不能满足要求，则会直接报错，而不是退回到较低版本
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 设置ON表示启用GCC有一些扩展功能；OFF则关闭GCC的扩展功能，只使用标准的C++
set(CMAKE_CXX_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)

link_directories(${CMAKE_SOURCE_DIR}/lib)

find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

if(APPLE)
    find_package(Freetype REQUIRED)
    message(STATUS "Found Freetype in ${FREETYPE_INCLUDE_DIRS}")
endif(APPLE)

if(WIN32)
    set(LIBS glfw3 opengl32 assimp freetype irrKlang)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
    find_package(OpenGL REQUIRED)
    add_definitions(${OPENGL_DEFINITIONS})
    find_package(X11 REQUIRED)
    set(LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread freetype ${ASSIMP_LIBRARY})

elseif(APPLE)

else()
    set(LIBS)
endif(WIN32)


set(CHAPTERS
        1.getting_started
        2.lighting
        3.model_loading
        4.advanced_opengl
)

set(1.getting_started
        1.1.hello_window
        1.2.hello_window_clear
        2.1.hello_triangle
        2.2.hello_triangle_indexed
        2.3.hello_triangle_exercise1
        2.4.hello_triangle_exercise2
        2.5.hello_triangle_exercise3
        3.1.shader_uniform
        3.2.shader_interpolation
        3.3.shader_class
        3.4.shader_exercise1
        3.5.shader_exercise2
        3.6.shader_exercise3
        4.1.texture
        4.2.texture_combined
        4.3.texture_exercise1
        4.4.texture_exercise2
        4.5.texture_exercise3
        4.6.texture_exercise4
        5.1.transformations
        5.2.transformations_exercise1
        5.3.transformations_exercise2
        6.1.coordinate_systems
        6.2.coordinate_systems_depth
        6.3.coordinate_systems_multiple
        7.1.camera_circle
        7.2.camera_keyboard
        7.3.camera_mouse_zoom
        7.4.camera_class
        7.5.camera_exercise1
        7.6.camera_exercise2
)

set(2.lighting
        1.color
        2.1.basic_lighting_diffuse
        2.2.basic_lighting_specular
        2.3.basic_lighting_exercise1
        2.4.basic_lighting_exercise2
        2.5.basic_lighting_exercise3
        3.1.materials
        3.2.material_exercise1
        4.1.lighting_maps_diffuse_map
        4.2.lighting_maps_specular_map
        4.3.lighting_maps_exercise2
        4.4.lighting_maps_exercise3
        4.5.lighting_maps_exercise4
        5.1.light_casters_directional
        5.2.light_casters_point
        5.3.light_casters_spot
        5.4.light_casters_spot_soft
        6.1.multiple_lights
        6.2.multiple_lights_exercise1
)


set(3.model_loading
        1.model_loading
)

set(4.advanced_opengl
        1.1.depth_testing
        1.2.depth_testing_view
)

# 将文件从一个地方复制到另一个地方并修改内容
configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

add_library(STB_IMAGE "src/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)

add_library(GLAD "src/glad.c")
set(LIBS ${LIBS} GLAD)

# imgui library
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/includes/imgui")
file(GLOB IMGUI_SOURCES ${IMGUI_DIR}/*.cpp ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp)
add_library(ImGui ${IMGUI_SOURCES})
target_link_libraries(ImGui glfw3)
target_include_directories(ImGui PUBLIC ${IMGUI_DIR})
set(LIBS ${LIBS} ImGui)

macro(makeLink src dest target)
    add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dest} DEPENDS ${dest} COMMENT "mklink ${src} -> ${dest}")
endmacro()

function(create_project_from_source chapter demo)
    message(STATUS "create_project_from_source ${chapter}/${demo}")
    file(GLOB SOURCE
            "src/${chapter}/${demo}/*.h"
            "src/${chapter}/${demo}/*.cpp"
            "src/${chapter}/${demo}/*.vert"
            "src/${chapter}/${demo}/*.frag"
            "src/${chapter}/${demo}/*.tcs"
            "src/${chapter}/${demo}/*.tes"
            "src/${chapter}/${demo}/*.gs"
            "src/${chapter}/${demo}/*.cs"
    )

    if(demo STREQUAL "")
        set(replaced "")
        # string(REPLACE <match_string> <replace_string> <output variable> <input> [<input>...])
        string(REPLACE "/" "_" replaced ${chapter})
        set(NAME ${replaced})
    else()
        set(NAME "${chapter}_${demo}")
    endif()

    add_executable(${NAME} ${SOURCE})
    target_link_libraries(${NAME} ${LIBS})

    if(MSVC)
        target_compile_options(${NAME} PRIVATE /std:c++17 /MP)
        target_link_options(${NAME} PUBLIC /ignore:4099)
    endif(MSVC)
    if(WIN32)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
        set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}/Debug")
    elseif(UNIX AND NOT APPLE)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
    elseif(APPLE)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/${chapter}")
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/${chapter}")
    endif(WIN32)

    # copy shader files to build directory
    file(GLOB SHADERS
            "src/${chapter}/${demo}/*.vert"
            "src/${chapter}/${demo}/*.frag"
            "src/${chapter}/${demo}/*.tcs"
            "src/${chapter}/${demo}/*.tes"
            "src/${chapter}/${demo}/*.gs"
            "src/${chapter}/${demo}/*.cs"
    )

    file(GLOB DLLS "dlls/*.dll")
    foreach(SHADER ${SHADERS})
        if(WIN32)
            add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${NAME}>)
            add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLLS} $<TARGET_FILE_DIR:${NAME}>)
        elseif(UNIX AND NOT APPLE)
            file(COPY ${SHADER} DESTINATION ${CMAKE_SOURCE_DIR}/bin/${chapter})
        elseif(APPLE)
            get_filename_component(SHADERNAME ${SHADER} NAME)
            makeLink(${SHADER} ${CMAKE_SOURCE_DIR}/bin/${chapter}/${SHADERNAME} ${NAME})
        endif(WIN32)
    endforeach(SHADER)

    if(MSVC)
        configure_file(${CMAKE_SOURCE_DIR}/configuration/visualstudio.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.vcxproj.user @ONLY)
    endif(MSVC)
endfunction()

foreach(CHAPTER ${CHAPTERS})
    message(STATUS "${CHAPTER}")
    foreach(DEMO ${${CHAPTER}})
        message(STATUS "${CHAPTER} ${DEMO}")
        create_project_from_source(${CHAPTER} ${DEMO})
    endforeach(DEMO)
endforeach(CHAPTER)

foreach(GUEST_ARTICLE ${GUEST_ARTICLES})

endforeach(GUEST_ARTICLE)

include_directories(${CMAKE_SOURCE_DIR}/includes)
