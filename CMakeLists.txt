cmake_minimum_required(VERSION 3.10)
project(sl_app LANGUAGES CXX)

# -------------------------
# Compiler settings
# -------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")

# -------------------------
# rpi-rgb-led-matrix paths
# -------------------------
set(RGBMATRIX_DIR ${CMAKE_SOURCE_DIR}/rpi-rgb-led-matrix)

set(RGBMATRIX_INCLUDE_DIR
    ${RGBMATRIX_DIR}/include
)

set(RGBMATRIX_LIBRARY
    ${RGBMATRIX_DIR}/lib/librgbmatrix.a
)

# Safety check
if(NOT EXISTS ${RGBMATRIX_LIBRARY})
    message(FATAL_ERROR
        "librgbmatrix.a not found. Run 'make' in rpi-rgb-led-matrix first.")
endif()

# -------------------------
# App target
# -------------------------
add_executable(sl_app
    src/main.cpp
    src/webrequest.cpp   # Add your new file here
)

# Include directories
target_include_directories(sl_app PRIVATE
    ${RGBMATRIX_INCLUDE_DIR}
    /usr/include          # for libcurl
    /usr/include/nlohmann # for nlohmann/json if installed via package
)

# Link libraries
target_link_libraries(sl_app
    ${RGBMATRIX_LIBRARY}
    curl   # link libcurl
    pthread
    rt
    m
)