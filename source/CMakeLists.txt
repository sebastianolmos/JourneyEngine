set(JOURNEY_HEADERS
        window/window.hpp
        input/inputController.hpp
        input/keyCodes.hpp
        input/joystickCodes.hpp
        engine.hpp
        application.hpp
        entity/entity.hpp
        entity/entityManager.hpp
        entity/entities/controlledEntity.hpp
        camera/camera.hpp
        camera/debugCamera.hpp
        camera/followCamera.hpp
        camera/cameraHandler.hpp
        component/component.hpp
        component/transform.hpp
        component/sprite.hpp
        component/staticMesh.hpp
        component/audioSource.hpp
        component/pointLight.hpp
        component/spotLight.hpp
        rendering/material.hpp
        rendering/meshManager.hpp
        rendering/renderManager.hpp
        rendering/textureManager.hpp
        rendering/shaders/shader.hpp
        rendering/meshGenerator.hpp
        rendering/mesh.hpp
        rendering/model.hpp
        rendering/importedModel.hpp
        audio/audioBuffer.hpp
        audio/audioManager.hpp
        scene/scene.hpp
        performanceMonitor.h)

set(JOURNEY_SOURCES
        window/window.cpp
        input/inputController.cpp
        scene/scene.cpp
        application.cpp
        entity/entityManager.cpp
        entity/entities/controlledEntity.cpp
        camera/debugCamera.cpp
        camera/followCamera.cpp
        component/transform.cpp
        component/audioSource.cpp
        component/pointLight.cpp
        component/spotLight.cpp
        rendering/material.cpp
        rendering/meshManager.cpp
        rendering/renderManager.cpp
        rendering/textureManager.cpp
        rendering/shaders/shader.cpp
        rendering/meshGenerator.cpp
        rendering/mesh.cpp
        rendering/model.cpp
        rendering/importedModel.cpp
        audio/audioBuffer.cpp
        audio/audioManager.cpp)

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