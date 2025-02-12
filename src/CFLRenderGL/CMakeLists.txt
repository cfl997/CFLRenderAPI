project(CFLRenderGL)
set(LIB_NAME CFLRenderGL)



SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CFLRender_ROOT_DIR}/bin64/plugin/RenderEngine")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CFLRender_ROOT_DIR}/bin64/plugin/RenderEngine")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CFLRender_ROOT_DIR}/bin64/plugin/RenderEngine")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CFLRender_ROOT_DIR}/bin64/plugin/RenderEngine")

add_definitions(-DCFLRENDERGL_EXPORTS)

include_directories(
	${CFLRender_ROOT_DIR}/include
	${CFLRender_ROOT_DIR}/include/CFLRenderGL
	${CFLRender_ROOT_DIR}/include/CFLRender
)

SET(COMMON_HEAD_FILES
	CFLRenderGL.h
	CFLMacros.h
	CFLInit.h
	CFLInclude.h
	CFLContext.h
	${CFLRender_ROOT_DIR}/include/CFLTime.h
)

SET(COMMON_SOURCE_FILES
	CFLRenderGL.cpp
	CFLInit.cpp
	CFLInclude.cpp
	CFLContext.cpp
	${CFLRender_ROOT_DIR}/src/CFLTime.cpp
)




if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
	SET(COMMON_SOURCE_FILES ${COMMON_SOURCE_FILES} CFLRenderGLD.def)
	add_definitions(-DDEBUG)
else()
	SET(COMMON_SOURCE_FILES ${COMMON_SOURCE_FILES} CFLRenderGL.def)
endif()


include_directories(
	${CFLRender_ROOT_DIR}/third/glew-2.2.0/include/
	${CFLRender_ROOT_DIR}/third/glm-1.0.1
	${CFLRender_ROOT_DIR}/include/CFLRenderGL
)

link_directories(
	${CFLRender_ROOT_DIR}/lib/win64/
	)

link_directories(
	${CFLRender_ROOT_DIR}/lib/win64/
	${CFLRender_ROOT_DIR}/third/glew-2.2.0/lib/Release/x64/

)


ADD_LIBRARY(${LIB_NAME} SHARED
	${COMMON_SOURCE_FILES} ${COMMON_HEAD_FILES} 
)


target_link_libraries(${PROJECT_NAME} ${CFLRender_ROOT_DIR}/third/glew-2.2.0/lib/Release/x64/glew32.lib )
target_link_libraries(${PROJECT_NAME} "opengl32.lib")

source_group(${CG_HEADER_FILES} FILES ${COMMON_HEAD_FILES})
source_group(${CG_SOURCE_FILES} FILES ${COMMON_SOURCE_FILES})
