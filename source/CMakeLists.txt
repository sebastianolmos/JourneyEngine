set(JOURNEY_HEADERS
        window/window.hpp
        input/inputController.hpp
        engine.hpp
        application.hpp
        entity/entity.hpp
        entity/entityManager.hpp
        entity/cameraHandler.hpp
        entity/pointLight.hpp
        component/component.hpp
        component/transform.hpp
        component/sprite.hpp
        component/staticMesh.hpp
        rendering/material.hpp
        rendering/meshManager.hpp
        rendering/renderManager.hpp
        rendering/textureManager.hpp
        rendering/shaders/shader.h
        scene/scene.hpp
        performanceMonitor.h)

set(JOURNEY_SOURCES
        application.cpp
        entity/entityManager.cpp
        component/transform.cpp
        rendering/meshManager.cpp
        rendering/renderManager.cpp
        rendering/textureManager.cpp
        window/window.cpp
        input/inputController.cpp
        scene/scene.cpp)

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