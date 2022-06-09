#pragma once

#include "../component/staticMesh.hpp"

namespace Journey {

    class MeshGenerator {
    
    public:
        void GeneratePlane(std::shared_ptr<StaticMeshModel> model, bool light);

        void GenerateCube(std::shared_ptr<StaticMeshModel> model, bool light);

        void GenerateCylinder(std::shared_ptr<StaticMeshModel> model, bool light, int segments);

        void GenerateSphere(std::shared_ptr<StaticMeshModel> model, bool light, int segments);
    };

}