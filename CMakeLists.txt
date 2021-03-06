set(CMAKE_LEGACY_CYGWIN_WIN32 OFF)
cmake_minimum_required(VERSION 3.15)
project(JourneyEngine C CXX)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
find_package(OpenGL REQUIRED)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(THIRD_PARTY_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/opengl3.3/source/glad/include"
									"${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/source/glfw/include"
									"${CMAKE_CURRENT_SOURCE_DIR}/libs/glm/source/glm"
									"${CMAKE_CURRENT_SOURCE_DIR}/libs/openal/source/openal-soft/include"
									"${CMAKE_CURRENT_SOURCE_DIR}/libs/dr_wav"
									"${CMAKE_CURRENT_SOURCE_DIR}/libs/assimp/source/assimp/include"
									"${CMAKE_CURRENT_SOURCE_DIR}/libs/stb")

set(JOURNEY_INCLUDE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/source")
if(NOT ${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
	set(THIRD_PARTY_INCLUDE_DIRECTORIES ${THIRD_PARTY_INCLUDE_DIRECTORIES} PARENT_SCOPE)
	set(JOURNEY_INCLUDE_DIRECTORY ${JOURNEY_INCLUDE_DIRECTORY} PARENT_SCOPE)
endif()

if(MSVC)
	set(THIRD_PARTY_LIBRARIES glad glfw ${OPENGL_LIBRARIES} OpenAL dr_wav assimp stb)
else()
	set(THIRD_PARTY_LIBRARIES glad glfw ${OPENGL_LIBRARIES} OpenAL dr_wav assimp stb)
endif(MSVC)

add_subdirectory(libs)
add_subdirectory(source)
add_subdirectory(games)