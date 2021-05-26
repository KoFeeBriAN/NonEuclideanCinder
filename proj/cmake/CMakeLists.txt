cmake_minimum_required( VERSION 3.10 FATAL_ERROR )
set( CMAKE_VERBOSE_MAKEFILE ON )

project( NonEuclideanCinder )

get_filename_component( CINDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../Cinder" ABSOLUTE )
get_filename_component( APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../" ABSOLUTE )

include( "${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake" )

file(GLOB SOURCES ../../src/*.cpp)
set ( RESOURCE_FILES
	${APP_PATH}/resources/environment_maps/humus_sf.jpg
)

ci_make_app(
	SOURCES		${SOURCES}
	INCLUDES	${APP_PATH}/include
	CINDER_PATH	${CINDER_PATH}
	RESOURCES	${RESOURCE_FILES}
)

# set ( SRC_FILES 
# 	${APP_PATH}/src/MainApp.cpp
# 	${APP_PATH}/src/CameraFP.cpp
# 	${APP_PATH}/src/Scene.cpp
# 	${APP_PATH}/src/SceneTest.cpp
# 	${APP_PATH}/src/SceneTest2.cpp
# 	${APP_PATH}/src/SceneRoom.cpp
# )