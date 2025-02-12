project(CFLRender)
set(LIB_NAME CFLRender)



SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CFLRender_ROOT_DIR}/bin64/")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CFLRender_ROOT_DIR}/bin64/")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CFLRender_ROOT_DIR}/bin64/")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CFLRender_ROOT_DIR}/bin64/")



add_definitions(-DCFL_GLRENDER_EXPORTS)


include_directories(
	${CFLRender_ROOT_DIR}/include
	${CFLRender_ROOT_DIR}/include/CFLRender
	${CFLRender_ROOT_DIR}/include/CFLRenderGL
	${CFLRender_ROOT_DIR}/third/freetype-2.13.0/Debug/include/freetype2
)

set(COMMON_HEAD_PUBLIC

	${CFLRender_ROOT_DIR}/include/CFLRender/CFLRender.h
	${CFLRender_ROOT_DIR}/include/CFLRender/CFLRenderAPI.h
	${CFLRender_ROOT_DIR}/include/CFLRender/CFLRenderMacros.h
	${CFLRender_ROOT_DIR}/include/CFLRender/CFLRender_Exports.h
	${CFLRender_ROOT_DIR}/include/CFLRender/CFLglm.h
	${CFLRender_ROOT_DIR}/include/CFLRender/CFLPath.h

)

SET(COMMON_HEAD_FILES
	CFLRender_Enums.h
	${CFLRender_ROOT_DIR}/include/CFLTime.h
)


SET(COMMON_SOURCE_FILES
	CFLRenderAPI.cpp
	CFLPath.cpp
	${CFLRender_ROOT_DIR}/src/CFLTime.cpp
)




if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	add_definitions("-DCFLRender_DEBUG")
else()
endif()


include_directories(
	${CFLRender_ROOT_DIR}/include/CFLRenderGL
	${CFLRender_ROOT_DIR}/third/glm-1.0.1	
)
link_directories(
	${CFLRender_ROOT_DIR}/lib/win64/
	)
link_directories(
	${CFLRender_ROOT_DIR}/lib/win64/
)

ADD_LIBRARY(${LIB_NAME} SHARED
	${COMMON_SOURCE_FILES} 
	${COMMON_HEAD_FILES} 
	${COMMON_HEAD_PUBLIC}
)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
else()
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug") 
target_link_libraries(${PROJECT_NAME} ${CFLRender_ROOT_DIR}/third/freetype-2.13.0/Debug/lib/freetyped.lib)
else()
target_link_libraries(${PROJECT_NAME} ${CFLRender_ROOT_DIR}/third/freetype-2.13.0/Release/lib/freetype.lib)
endif()

add_dependencies(${PROJECT_NAME} CFLRenderGL)

source_group(${CG_HEADER_FILES} FILES ${COMMON_HEAD_FILES})
source_group(${CG_SOURCE_FILES} FILES ${COMMON_SOURCE_FILES})
source_group("Public" FILES ${COMMON_HEAD_PUBLIC})
