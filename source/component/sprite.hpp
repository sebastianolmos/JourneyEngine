#pragma once

#include "component.hpp"
#include "../rendering/material.hpp"
#include "../rendering/renderManager.hpp"

namespace Journey {
    struct Material;
    class SpriteComponent : public Component {
        public:
            SpriteComponent() = default;

            float* vertices;
            int vertexCount;
            int vertexStride;

            unsigned int VAO;
            unsigned int VBO;
            unsigned textureID;

            std::shared_ptr<Material> material;

            virtual EComponentType GetType() override { return EComponentType::SpriteComponent; }
            
            virtual void UpdateRenderInfo(RenderInfo& rInfo) override {
                rInfo.VAO = VAO;
                rInfo.VBO = VBO;
                rInfo.vertexCount = vertexCount;
                if (material->GetType() == EMaterialType::SimpleTextured) 
                    rInfo.textureID = textureID;
                switch(material->GetType()) {
                    case EMaterialType::SimpleColored:
                        rInfo.ks = dynamic_cast<SimpleColoredMaterial*>((material).get())->color;
                        break;
                    case EMaterialType::SimpleTextured:
                        rInfo.textureID = textureID;
                        break;
                    case EMaterialType::FlatColored:
                        rInfo.ks = dynamic_cast<FlatColoredMaterial*>(material.get())->ks;
                        rInfo.kd = dynamic_cast<FlatColoredMaterial*>(material.get())->kd;
                        rInfo.ke = dynamic_cast<FlatColoredMaterial*>(material.get())->ke;
                        break;
                    case EMaterialType::PhongColored:
                        rInfo.ks = dynamic_cast<PhongColoredMaterial*>(material.get())->ks;
                        rInfo.kd = dynamic_cast<PhongColoredMaterial*>(material.get())->kd;
                        rInfo.ke = dynamic_cast<PhongColoredMaterial*>(material.get())->ke;
                        break;
                    case EMaterialType::FlatTextured:
                        rInfo.ks = dynamic_cast<FlatTexturedMaterial*>(material.get())->ks;
                        rInfo.kd = dynamic_cast<FlatTexturedMaterial*>(material.get())->kd;
                        rInfo.ke = dynamic_cast<FlatTexturedMaterial*>(material.get())->ke;
                        break;
                    case EMaterialType::PhongTextured:
                        rInfo.ks = dynamic_cast<PhongTexturedMaterial*>(material.get())->ks;
                        rInfo.kd = dynamic_cast<PhongTexturedMaterial*>(material.get())->kd;
                        rInfo.ke = dynamic_cast<PhongTexturedMaterial*>(material.get())->ke;
                        break;
                    default:
                        // code block
                        break;
                }               
            }
    };

}