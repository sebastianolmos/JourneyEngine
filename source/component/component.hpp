#pragma once
#include <cstdint>

namespace Journey {

    enum class EComponentType : uint8_t {
		StaticMeshComponent,
        SpriteComponent
	};

    class Component {
        public:
            EComponentType GetType() {return mType;}
            virtual void A() {}

        protected:
            EComponentType mType;        
    };

}