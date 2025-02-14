cmake_minimum_required(VERSION 3.18)


project(gTestCFLRenderGL)


include_directories(
	"${CFLRender_ROOT_DIR}/include"
	${CFLRender_ROOT_DIR}/include/CFLRender
	${CFLRender_ROOT_DIR}/third/glm-1.0.1
	${CFLRender_ROOT_DIR}/third/gTest/Debug/include
	${CFLRender_ROOT_DIR}/third/glfw-3.4/include
	
)

link_directories(
	${CFLRender_ROOT_DIR}/lib/win64/
)



set(common_header
	gTestInclude.h
	gTestEnvironment.h
)

set(common_src
	gTestInclude.cpp
	gTestRenderAPI.cpp
)


add_executable(${PROJECT_NAME}
	${common_src}
	${common_header}
)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	target_link_libraries(${PROJECT_NAME} ${CFLRender_ROOT_DIR}/third/gTest/Debug/lib/gtest.lib)
	target_link_libraries(${PROJECT_NAME} ${CFLRender_ROOT_DIR}/third/gTest/Debug/lib/gtest_main.lib)
	target_link_libraries(${PROJECT_NAME} ${CFLRender_ROOT_DIR}/third/glfw-3.4/lib/Debug/glfw3.lib )
else()
	target_link_libraries(${PROJECT_NAME} ${CFLRender_ROOT_DIR}/third/gTest/Release/lib/gtest.lib)
	target_link_libraries(${PROJECT_NAME} ${CFLRender_ROOT_DIR}/third/gTest/Release/lib/gtest_main.lib)
	target_link_libraries(${PROJECT_NAME} ${CFLRender_ROOT_DIR}/third/glfw-3.4/lib/Release/glfw3.lib )
endif()

SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY FOLDER "gTest")

add_dependencies(${PROJECT_NAME} CFLRender)
