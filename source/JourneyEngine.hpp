#pragma once

#ifndef JOURNEYENGINE_HPP
#define JOURNEYENGINE_HPP

#include "engine.hpp"
#include "input/keyCodes.hpp"
#include "input/joystickCodes.hpp"
#include "application.hpp"
#include "scene/scene.hpp"
#include "rendering/meshManager.hpp"
#include "rendering/textureManager.hpp"
#include "rendering/renderManager.hpp"
#include "component/audioSource.hpp"
#include "component/pointLight.hpp"
#include "component/spotLight.hpp"
#include "component/skeletalMesh.hpp"
#include "skeletal/skeletalModel.hpp"
#include "skeletal/skeletalManager.hpp"
#include "audio/audioManager.hpp"
#include "camera/followCamera.hpp"
#include "camera/cameraHandler.hpp"
#include "entity/entities/controlledEntity.hpp"
#include <memory>
#include <stdlib.h>
#include <ctime>
#include  <glm/gtx/norm.hpp>

#endif