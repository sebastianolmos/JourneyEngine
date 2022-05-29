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

        virtual EMaterialType GetType() = 0;
        virtual void A() {}
    };

    struct SimpleColoredMaterial : public Material {
        glm::fvec3 color;

        virtual EMaterialType GetType() override{ return EMaterialType::SimpleColored;}
    };

    struct SimpleTexturedMaterial : public Material {

        virtual EMaterialType GetType() override{ return EMaterialType::SimpleTextured;}
    };

    struct FlatColoredMaterial : public Material {
        glm::fvec3 ks;
        glm::fvec3 kd;
        glm::fvec3 ke;

        virtual EMaterialType GetType() override{ return EMaterialType::FlatColored;}
    };

    struct PhongColoredMaterial : public Material {
        glm::fvec3 ks;
        glm::fvec3 kd;
        glm::fvec3 ke;

        virtual EMaterialType GetType() override{ return EMaterialType::PhongColored;}
    };

    struct FlatTexturedMaterial : public Material {
        glm::fvec3 ks;
        glm::fvec3 kd;
        glm::fvec3 ke;

        virtual EMaterialType GetType() override{ return EMaterialType::PhongColored;}
    };

    struct PhongTexturedMaterial : public Material {
        glm::fvec3 ks;
        glm::fvec3 kd;
        glm::fvec3 ke;

        virtual EMaterialType GetType() override{ return EMaterialType::PhongTextured;}
    };

}