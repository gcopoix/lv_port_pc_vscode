cmake_minimum_required(VERSION 3.12)
project (lv_demo)

set(DISPLAY_DRIVER   "SDL"             CACHE STRING "Selected display driver ('SDL' or 'X11')")
set(LVGL_CHOSEN_DEMO "lv_demo_widgets" CACHE STRING "Selected LVGL demo (see 'lvgl/demos' folder)")

add_compile_options(-Wall -Werror -Wextra -Wpedantic)
include_directories(${PROJECT_SOURCE_DIR})
set(DEFINES LV_USE_${DISPLAY_DRIVER} LV_USE_PERF_MONITOR=1 LV_USE_MEM_MONITOR=1 LV_USE_SYSMON=1 LV_USE_OBSERVER=1)

add_subdirectory (lvgl)

file (GLOB MY_SOURCES "main/src/*.c")
set (SOURCES ${MY_SOURCES} )

add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDES})

find_package(PkgConfig)
if (DISPLAY_DRIVER STREQUAL "SDL")
    pkg_check_modules(SDL2 sdl2)
    set (DRIVERLIB SDL2)
elseif (DISPLAY_DRIVER STREQUAL "X11")
    pkg_check_modules (X11 x11)
    set (DRIVERLIB X11)
endif()

message (STATUS "LVGL settings:")
message (STATUS "  Using display driver '${DISPLAY_DRIVER}'")
message (STATUS "  Additional compiler defines: '${DEFINES}'")
message (STATUS "LVGL sample:")
message (STATUS "  Using sample demonstration '${LVGL_CHOSEN_DEMO}()'")
target_compile_definitions(lvgl            PRIVATE LV_USE_${DISPLAY_DRIVER} ${DEFINES})
target_compile_definitions(${PROJECT_NAME} PRIVATE LV_USE_${DISPLAY_DRIVER} ${DEFINES})
target_compile_definitions(${PROJECT_NAME} PRIVATE CHOSEN_DEMO=${LVGL_CHOSEN_DEMO})

target_link_libraries(${PROJECT_NAME}
    lvgl
    lvgl_examples
    lvgl_demos
    pthread
    ${DRIVERLIB}
)
