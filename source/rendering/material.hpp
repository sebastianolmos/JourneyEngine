#pragma once
#include <cstdint>
#include <glm/glm.hpp>

namespace Journey {
    struct RenderInfo;
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
        virtual void fillRenderInfo(RenderInfo& rInfo) {}
    };

    struct SimpleColoredMaterial : public Material {
        glm::fvec3 color;

        virtual EMaterialType GetType() override;
        virtual void fillRenderInfo(RenderInfo& rInfo) override;
    };

    struct SimpleTexturedMaterial : public Material {

        virtual EMaterialType GetType() override;
        virtual void fillRenderInfo(RenderInfo& rInfo) override;
    };

    struct FlatColoredMaterial : public Material {
        glm::fvec3 ks;
        glm::fvec3 kd;
        glm::fvec3 ke;
        glm::fvec3 color;

        virtual EMaterialType GetType() override;
        virtual void fillRenderInfo(RenderInfo& rInfo) override;
    };

    struct PhongColoredMaterial : public Material {
        glm::fvec3 ks;
        glm::fvec3 kd;
        glm::fvec3 ke;
        glm::fvec3 color;

        virtual EMaterialType GetType() override;
        virtual void fillRenderInfo(RenderInfo& rInfo) override;
    };

    struct FlatTexturedMaterial : public Material {
        glm::fvec3 ks;
        glm::fvec3 kd;
        glm::fvec3 ke;

        virtual EMaterialType GetType() override;
        virtual void fillRenderInfo(RenderInfo& rInfo) override;
    };

    struct PhongTexturedMaterial : public Material {
        glm::fvec3 ks;
        glm::fvec3 kd;
        glm::fvec3 ke;

        virtual EMaterialType GetType() override;
        virtual void fillRenderInfo(RenderInfo& rInfo) override;
    };

}