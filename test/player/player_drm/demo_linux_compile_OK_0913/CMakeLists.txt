cmake_minimum_required(VERSION 3.8)

project(png_demo)

add_definitions(-g)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include_directories(
	${PROJECT_SOURCE_DIR}/./
	${CMAKE_SYSROOT}/usr/include/libdrm/
	${CMAKE_SYSROOT}/usr/include/lvgl/
	${CMAKE_SYSROOT}/usr/include/
	)

add_executable(dec_drm dec_drm.c drm_display.c)
target_link_libraries(dec_drm
	drm lvgl pthread m
)
install(TARGETS dec_drm RUNTIME DESTINATION bin)
