
find_path(ASSIMP_INCLUDE_DIR
        assimp/mesh.h
        /usr/include
        /usr/local/include
        /opt/local/include
        ${CMAKE_SOURCE_DIR}/includes
)


find_library(ASSIMP_LIBRARY assimp
        /usr/lib64
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        ${CMAKE_SOURCE_DIR}/lib
)

if(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY)
    set(ASSIMP_FOUND TRUE)
    set(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY})
endif(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY)

if(ASSIMP_FOUND)
    if(NOT ASSIMP_FIND_QUIETLY)
        message(STATUS "Found ASSIMP: ${ASSIMP_LIBRARIES}")
    endif(NOT ASSIMP_FIND_QUIETLY)
else(ASSIMP_FOUND)
    if(ASSIMP_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find libAssimp")
    endif(ASSIMP_FIND_REQUIRED)
endif(ASSIMP_FOUND)