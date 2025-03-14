cmake_minimum_required(VERSION 3.15)
project(HavenProject LANGUAGES C)

# Set C standard and properties
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Configuration options
option(BUILD_EDITOR "Build the Haven Editor" ON)

# Project structure
set(LIB_NAME HavenLib)
set(EXEC_NAME Game)
set(EDITOR_NAME HavenEditor)

# Source files
file(GLOB ENTITY_SRC "source/entity/*.c")
file(GLOB AUDIO_SRC "source/audio/*.c")
file(GLOB CORE_SRC "source/core/*.c")
file(GLOB DEBUG_SRC "source/debug/*.c")
file(GLOB PHYSICS_SRC "source/physics/*.c")
file(GLOB ITEM_SRC "source/item/*.c")
file(GLOB RENDER_SRC "source/render/*.c")
file(GLOB SCENE_SRC "source/scene/*.c")

set(LIB_SRC
    ${ENTITY_SRC}
    ${AUDIO_SRC}
    ${CORE_SRC}
    ${DEBUG_SRC}
    ${PHYSICS_SRC}
    ${ITEM_SRC}
    ${RENDER_SRC}
    ${SCENE_SRC}
)

# Executable sources
set(EXEC_SRC source/main.c)
set(EDITOR_SRC source/editor.c)

# Include directories
include_directories(include)

# Platform-specific configuration
if(WIN32)
    # Windows-specific settings
    include_directories("C:/mingw64/include")
    set(SYSTEM_LIBS glfw vulkan raylib gl gdi32 winmm)
elseif(UNIX AND NOT APPLE)
    # Linux-specific settings
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    set(SYSTEM_LIBS glfw vulkan X11 pthread dl rt m)
endif()

# Create the static library
add_library(${LIB_NAME} STATIC ${LIB_SRC})
target_link_libraries(${LIB_NAME} PRIVATE ${SYSTEM_LIBS})

# Create game executable
add_executable(${EXEC_NAME} ${EXEC_SRC})
target_link_libraries(${EXEC_NAME} PRIVATE ${LIB_NAME} ${SYSTEM_LIBS})

# Create editor executable if enabled
if(BUILD_EDITOR)
    add_executable(${EDITOR_NAME} ${EDITOR_SRC})
    target_link_libraries(${EDITOR_NAME} PRIVATE ${LIB_NAME} ${SYSTEM_LIBS})
endif()

# Installation setup (optional)
install(TARGETS ${LIB_NAME} ${EXEC_NAME} ${EDITOR_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
