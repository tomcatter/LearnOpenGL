
set(_glfw3_HEADER_SEARCH_DIRS
        "/usr/include"
        "/usr/local/include"
        "${CMAKE_SOURCE_DIR}/includes"
        "C:/Program Files (x86)/glfw/include"
)

set(_glfw3_LIB_SEARCH_DIRS
        "/usr/lib"
        "/usr/local/lib"
        "${CMAKE_SOURCE_DIR}/lib"
        "C:/Program Files (x86)/glfw/lib-msvc110"
)

# 检查环境变量
set(_glfw3_ENV_ROOT $ENV{GLFW3_ROOT})

message(STATUS "_glfw3_ENV_ROOT=${_glfw3_ENV_ROOT}")

if(NOT GLFW3_ROOT AND _glfw3_ENV_ROOT)
    set(GLFW3_ROOT ${_glfw3_ENV_ROOT})
endif(NOT GLFW3_ROOT AND _glfw3_ENV_ROOT)

if(GLFW3_ROOT)
    list(INSERT _glfw3_HEADER_SEARCH_DIRS 0 "${GLFW3_ROOT}/include")
    list(INSERT _glfw3_LIB_SEARCH_DIRS 0 "${GLFW3_ROOT}/lib")
endif(GLFW3_ROOT)

find_path(GLFW3_INCLUDE_DIR "GLFW/glfw3.h" PATHS ${_glfw3_HEADER_SEARCH_DIRS})

find_library(GLFW3_LIBRARY glfw3 glfw PATHS ${_glfw3_LIB_SEARCH_DIRS})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(GLFW3 DEFAULT_MSG GLFW3_LIBRARY GLFW3_INCLUDE_DIR)
