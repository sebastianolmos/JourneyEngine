#pragma once
#include <cstdint>

#include <glm/glm.hpp>

namespace Journey {

    enum class EMaterialType : uint8_t {
		SimpleColored,
        SimpleTextured,
        FlatColored,
        PhongColored,
        FlatTextured,
        PhongTextured
	};

    struct Material {
        EMaterialType mType;

        EMaterialType GetType() {return mType;}
        virtual void A() {}
    };

    struct SimpleColoredMaterial : Material {
        glm::fvec3 color;
    };

    struct SimpleTexturedMaterial : Material {
    };

    struct FlatColoredMaterial : Material {
        glm::fvec3 ks;
        glm::fvec3 kd;
        glm::fvec3 ke;
    };

    struct PhongColoredMaterial : Material {
        glm::fvec3 ks;
        glm::fvec3 kd;
        glm::fvec3 ke;
    };

    struct FlatTexturedMaterial : Material {
        glm::fvec3 ks;
        glm::fvec3 kd;
        glm::fvec3 ke;
    };

    struct PhongTexturedMaterial : Material {
        glm::fvec3 ks;
        glm::fvec3 kd;
        glm::fvec3 ke;
    };

}