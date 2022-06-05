#pragma once

#include "../component/staticMesh.hpp"

namespace Journey {

    class MeshGenerator {
    
    public:
        void GeneratePlane(std::shared_ptr<StaticMeshComponent> component, bool light);

        void GenerateCube(std::shared_ptr<StaticMeshComponent> component, bool light);

        void GenerateCylinder(std::shared_ptr<StaticMeshComponent> component, bool light, int segments);

        void GenerateSphere(std::shared_ptr<StaticMeshComponent> component, bool light, int segments);
    };

}