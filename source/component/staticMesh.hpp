#pragma once

#include "component.hpp"
#include "../rendering/material.hpp"
#include "../rendering/renderManager.hpp"

namespace Journey {
    class StaticMeshComponent : public Component {
        public:
            StaticMeshComponent() = default;

            float* vertices;
            int vertexCount;
            int vertexStride;

            unsigned int VAO;
            unsigned int VBO;
            unsigned textureID;

            std::shared_ptr<Material> material;

            virtual EComponentType GetType() override { return EComponentType::StaticMeshComponent; }

            
            virtual void UpdateRenderInfo(RenderInfo& rInfo) override {
                rInfo.VAO = VAO;
                rInfo.VBO = VBO;
                rInfo.vertexCount = vertexCount;
                switch(material->GetType()) {
                    case EMaterialType::SimpleColored:
                        rInfo.ks = dynamic_cast<SimpleColoredMaterial*>((material).get())->color;
                        break;
                    case EMaterialType::SimpleTextured:
                        break;
                    case EMaterialType::FlatColored:
                        rInfo.ks = dynamic_cast<FlatColoredMaterial*>((material).get())->ks;
                        rInfo.kd = dynamic_cast<FlatColoredMaterial*>((material).get())->kd;
                        rInfo.ke = dynamic_cast<FlatColoredMaterial*>((material).get())->ke;
                        break;
                    case EMaterialType::PhongColored:
                        rInfo.ks = dynamic_cast<PhongColoredMaterial*>((material).get())->ks;
                        rInfo.kd = dynamic_cast<PhongColoredMaterial*>((material).get())->kd;
                        rInfo.ke = dynamic_cast<PhongColoredMaterial*>((material).get())->ke;
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