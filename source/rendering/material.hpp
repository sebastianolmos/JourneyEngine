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

    struct SimpleColoredMaterial : public Material {
        glm::fvec3 color;
    };

    struct SimpleTexturedMaterial : public Material {
    };

    struct FlatColoredMaterial : public Material {
        glm::fvec3 ks;
        glm::fvec3 kd;
        glm::fvec3 ke;
    };

    struct PhongColoredMaterial : public Material {
        glm::fvec3 ks;
        glm::fvec3 kd;
        glm::fvec3 ke;
    };

    struct FlatTexturedMaterial : public Material {
        glm::fvec3 ks;
        glm::fvec3 kd;
        glm::fvec3 ke;
    };

    struct PhongTexturedMaterial : public Material {
        glm::fvec3 ks;
        glm::fvec3 kd;
        glm::fvec3 ke;
    };

}