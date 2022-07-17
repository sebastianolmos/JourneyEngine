#pragma once
#include <cstdint>

namespace Journey {
    struct RenderInfo;
    enum class EComponentType : uint8_t {
		StaticMeshComponent,
        SpriteComponent,
        AudioSourceComponent,
        PointLightComponent,
        SpotLightComponent
	};

    class Component {
        public:
            virtual EComponentType GetType() = 0;
            virtual void UpdateRenderInfo(RenderInfo& rInfo) {}
            
            EComponentType mType;        
    };

}