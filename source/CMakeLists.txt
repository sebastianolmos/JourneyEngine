set(JOURNEY_HEADERS
        application.hpp
        shaders/shader.h
        performanceMonitor.h)

set(JOURNEY_SOURCES
        application.cpp)

file(GLOB_RECURSE Shaders  "${CMAKE_SOURCE_DIR}/source/shaders/*")
source_group("Shaders" FILES ${Shaders})

add_library(JourneyEngine STATIC ${JOURNEY_SOURCES} ${JOURNEY_HEADERS} JourneyEngine.hpp ${Shaders})

if (MSVC)
    target_compile_options(JourneyEngine PUBLIC /wd5033)
endif(MSVC)

target_include_directories(JourneyEngine PRIVATE ${THIRD_PARTY_INCLUDE_DIRECTORIES} JOURNEY_INCLUDE_DIRECTORY)
target_link_libraries(JourneyEngine PRIVATE ${THIRD_PARTY_LIBRARIES})
set_property(TARGET JourneyEngine PROPERTY CXX_STANDARD 20)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${JOURNEY_SOURCES} ${JOURNEY_HEADERS})