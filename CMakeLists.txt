cmake_minimum_required(VERSION 3.5)
project(Renderer)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(HEADER_FILES
    core/image/image.h
    core/maths/maths.h
    core/texture/texture.h
    core/utils/utils.h
    core/draw2d/draw2d.h
    core/mesh/mesh.h
    core/darray/darray.h
    core/win32/win32.h
    core/model/model.h
    core/graphics/graphics.h
    shader/flat/flat_shader.h
    shader/gouraud/gouraud_shader.h
    core/api.h
    core/common.h
    core/macro.h
    shader/shader.h
)

set(SOURCE_FILES
    core/image/image.c
    core/maths/maths.c    
    core/texture/texture.c
    core/utils/utils.c
    core/draw2d/draw2d.c
    core/mesh/mesh.c
    core/win32/win32.c
    core/model/model.c
    core/graphics/graphics.c
    shader/flat/flat_shader.c
    shader/gouraud/gouraud_shader.c
)

set(SRC_CUR test/test_gouraud.c)
add_executable(Renderer ${HEADER_FILES} ${SRC_CUR}  
                          ${SOURCE_FILES})