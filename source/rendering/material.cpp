#include "material.hpp"
#include "renderManager.hpp"

namespace Journey {
    EMaterialType SimpleColoredMaterial::GetType() { 
        return EMaterialType::SimpleColored;
    }
    void SimpleColoredMaterial::fillRenderInfo(RenderInfo& rInfo) {
        rInfo.ks = color;
    }

    EMaterialType SimpleTexturedMaterial::GetType() { 
        return EMaterialType::SimpleTextured;
    }
    void SimpleTexturedMaterial::fillRenderInfo(RenderInfo& rInfo) {}

    EMaterialType FlatColoredMaterial::GetType() { 
        return EMaterialType::FlatColored;
    }
    void FlatColoredMaterial::fillRenderInfo(RenderInfo& rInfo) {
        rInfo.ks = ks;
        rInfo.kd = kd;
        rInfo.ke = ke;
        rInfo.color = color;
    }

    EMaterialType PhongColoredMaterial::GetType() { 
        return EMaterialType::PhongColored;
    }
    void PhongColoredMaterial::fillRenderInfo(RenderInfo& rInfo) {
        rInfo.ks = ks;
        rInfo.kd = kd;
        rInfo.ke = ke;
        rInfo.color = color;
    }

    EMaterialType FlatTexturedMaterial::GetType() { 
        return EMaterialType::FlatTextured;
    }
    void FlatTexturedMaterial::fillRenderInfo(RenderInfo& rInfo) {
        rInfo.ks = ks;
        rInfo.kd = kd;
        rInfo.ke = ke;
    }

    EMaterialType PhongTexturedMaterial::GetType() { 
        return EMaterialType::PhongTextured;
    }
    void PhongTexturedMaterial::fillRenderInfo(RenderInfo& rInfo) {
        rInfo.ks = ks;
        rInfo.kd = kd;
        rInfo.ke = ke;
    }
}