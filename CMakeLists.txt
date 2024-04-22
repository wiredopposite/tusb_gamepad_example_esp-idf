# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# include($ENV{IDF_PATH}/tools/cmake/project.cmake)
# project(blink)

# set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/components)

set(FAMILY espressif)
set(BOARD espressif_s3_devkitm)

set(TINYUSB_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/lib/tinyusb)
include(${TINYUSB_ROOT}/hw/bsp/family_support.cmake)

family_get_project_name(PROJECT ${CMAKE_CURRENT_LIST_DIR})

project(${PROJECT} C CXX ASM)

family_initialize_project(${PROJECT} ${CMAKE_CURRENT_LIST_DIR})