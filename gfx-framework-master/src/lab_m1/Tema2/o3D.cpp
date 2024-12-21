#include "lab_m1/Tema2/o3D.h"
#include <vector>
#include <iostream>

Mesh* o3D::CreateTerrain(const std::string& name, int rows, int cols, float spacing, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Geneterez vertecsii
    for (int i = 0; i <= rows; ++i) {
        for (int j = 0; j <= cols; ++j) {
            float x = j * spacing;
            float z = i * spacing;
            vertices.emplace_back(glm::vec3(x, 0.0f, z), color);
        }
    }

    // Indici pentru triangle grid
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int topLeft = i * (cols + 1) + j;
            int topRight = topLeft + 1;
            int bottomLeft = (i + 1) * (cols + 1) + j;
            int bottomRight = bottomLeft + 1;

            // Primul triunghi
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Al doilea triunghi
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    Mesh* terrainMesh = new Mesh(name);
    terrainMesh->InitFromData(vertices, indices);
    return terrainMesh;
}

Mesh* o3D::CreateCube(const std::string& name, glm::vec3 center, float sideLength, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    float halfSide = sideLength / 2.0f;

    // Vertecsi cub
    vertices = {
        VertexFormat(center + glm::vec3(-halfSide, -halfSide, -halfSide), color),  // 0: Bottom-left-front
        VertexFormat(center + glm::vec3(halfSide, -halfSide, -halfSide), color),   // 1: Bottom-right-front
        VertexFormat(center + glm::vec3(halfSide, halfSide, -halfSide), color),    // 2: Top-right-front
        VertexFormat(center + glm::vec3(-halfSide, halfSide, -halfSide), color),   // 3: Top-left-front
        VertexFormat(center + glm::vec3(-halfSide, -halfSide, halfSide), color),   // 4: Bottom-left-back
        VertexFormat(center + glm::vec3(halfSide, -halfSide, halfSide), color),    // 5: Bottom-right-back
        VertexFormat(center + glm::vec3(halfSide, halfSide, halfSide), color),     // 6: Top-right-back
        VertexFormat(center + glm::vec3(-halfSide, halfSide, halfSide), color)     // 7: Top-left-back
    };

    // indicii pentru cele 6 fete
    indices = {
        0, 1, 2, 2, 3, 0,  // Front face
        1, 5, 6, 6, 2, 1,  // Right face
        5, 4, 7, 7, 6, 5,  // Back face
        4, 0, 3, 3, 7, 4,  // Left face
        3, 2, 6, 6, 7, 3,  // Top face
        0, 1, 5, 5, 4, 0   // Bottom face
    };

    Mesh* cubeMesh = new Mesh(name);
    cubeMesh->InitFromData(vertices, indices);
    return cubeMesh;
}



Mesh* o3D::CreateParallelepiped(const std::string& name, glm::vec3 center, glm::vec3 dimensions, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 halfDim = dimensions / 2.0f;

    vertices = {
        VertexFormat(center + glm::vec3(-halfDim.x, -halfDim.y, -halfDim.z), color),  // 0: Bottom-left-front
        VertexFormat(center + glm::vec3(halfDim.x, -halfDim.y, -halfDim.z), color),   // 1: Bottom-right-front
        VertexFormat(center + glm::vec3(halfDim.x, halfDim.y, -halfDim.z), color),    // 2: Top-right-front
        VertexFormat(center + glm::vec3(-halfDim.x, halfDim.y, -halfDim.z), color),   // 3: Top-left-front
        VertexFormat(center + glm::vec3(-halfDim.x, -halfDim.y, halfDim.z), color),   // 4: Bottom-left-back
        VertexFormat(center + glm::vec3(halfDim.x, -halfDim.y, halfDim.z), color),    // 5: Bottom-right-back
        VertexFormat(center + glm::vec3(halfDim.x, halfDim.y, halfDim.z), color),     // 6: Top-right-back
        VertexFormat(center + glm::vec3(-halfDim.x, halfDim.y, halfDim.z), color)     // 7: Top-left-back
    };

    indices = {
        0, 1, 2, 2, 3, 0,  // Front face
        1, 5, 6, 6, 2, 1,  // Right face
        5, 4, 7, 7, 6, 5,  // Back face
        4, 0, 3, 3, 7, 4,  // Left face
        3, 2, 6, 6, 7, 3,  // Top face
        0, 1, 5, 5, 4, 0   // Bottom face
    };

    Mesh* parallelepipedMesh = new Mesh(name);
    parallelepipedMesh->InitFromData(vertices, indices);
    return parallelepipedMesh;
}

Mesh* o3D::CreateOutlineParallelepiped(const std::string& name, glm::vec3 center, glm::vec3 dimensions, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 halfDim = dimensions / 2.0f;

    vertices = {
        VertexFormat(center + glm::vec3(-halfDim.x, -halfDim.y, -halfDim.z), color),  // 0: Bottom-left-front
        VertexFormat(center + glm::vec3(halfDim.x, -halfDim.y, -halfDim.z), color),   // 1: Bottom-right-front
        VertexFormat(center + glm::vec3(halfDim.x, halfDim.y, -halfDim.z), color),    // 2: Top-right-front
        VertexFormat(center + glm::vec3(-halfDim.x, halfDim.y, -halfDim.z), color),   // 3: Top-left-front
        VertexFormat(center + glm::vec3(-halfDim.x, -halfDim.y, halfDim.z), color),   // 4: Bottom-left-back
        VertexFormat(center + glm::vec3(halfDim.x, -halfDim.y, halfDim.z), color),    // 5: Bottom-right-back
        VertexFormat(center + glm::vec3(halfDim.x, halfDim.y, halfDim.z), color),     // 6: Top-right-back
        VertexFormat(center + glm::vec3(-halfDim.x, halfDim.y, halfDim.z), color)     // 7: Top-left-back
    };

    indices = {
        0, 1, 1, 2, 2, 3, 3, 0,  // Front face edges
        4, 5, 5, 6, 6, 7, 7, 4,  // Back face edges
        0, 4, 1, 5, 2, 6, 3, 7   // Connecting edges
    };

    Mesh* outlineMesh = new Mesh(name);
    outlineMesh->SetDrawMode(GL_LINES);
    outlineMesh->InitFromData(vertices, indices);
    return outlineMesh;
}


Mesh* o3D::CreateCylinder(const std::string& name, int segments, float radius, float height, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // vertecsi pentru Cercurile de jos si sus
    for (int i = 0; i <= segments; ++i) {
        float theta = i * 2.0f * glm::pi<float>() / segments;
        float x = radius * cos(theta);
        float z = radius * sin(theta);

        // jos
        vertices.emplace_back(glm::vec3(x, 0.0f, z), color);
        // sus
        vertices.emplace_back(glm::vec3(x, height, z), color);
    }

    // umplu centrele
    vertices.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), color);
    vertices.emplace_back(glm::vec3(0.0f, height, 0.0f), color);

    // indici pentru margini
    for (int i = 0; i < segments; ++i) {
        int bottom1 = 2 * i;
        int top1 = 2 * i + 1;
        int bottom2 = 2 * (i + 1) % (2 * segments);
        int top2 = 2 * (i + 1) % (2 * segments) + 1;

        indices.push_back(bottom1);
        indices.push_back(bottom2);
        indices.push_back(top1);

        indices.push_back(top1);
        indices.push_back(bottom2);
        indices.push_back(top2);
    }

    // indici pentru cercuri
    int bottomCenter = 2 * segments;
    int topCenter = 2 * segments + 1;
    for (int i = 0; i < segments; ++i) {
        int bottom = 2 * i;
        int top = 2 * i + 1;

        // jos
        indices.push_back(bottomCenter);
        indices.push_back(bottom);
        indices.push_back((bottom + 2) % (2 * segments));

        // sus
        indices.push_back(topCenter);
        indices.push_back((top + 2) % (2 * segments));
        indices.push_back(top);
    }

    Mesh* cylinderMesh = new Mesh(name);
    cylinderMesh->InitFromData(vertices, indices);
    return cylinderMesh;
}

Mesh* o3D::CreateCone(const std::string& name, int segments, float radius, float height, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // baza
    for (int i = 0; i <= segments; ++i) {
        float theta = i * 2.0f * glm::pi<float>() / segments;
        float x = radius * cos(theta);
        float z = radius * sin(theta);
        vertices.emplace_back(glm::vec3(x, 0.0f, z), color);
    }

    // varful
    vertices.emplace_back(glm::vec3(0.0f, height, 0.0f), color);

    // centrul bazei
    vertices.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), color);

    int tipIndex = segments + 1;
    int centerIndex = segments + 2;

    // indicii de margine
    for (int i = 0; i < segments; ++i) {
        indices.push_back(i);
        indices.push_back((i + 1) % segments);
        indices.push_back(tipIndex);
    }

    // indicii pentru baza
    for (int i = 0; i < segments; ++i) {
        indices.push_back(centerIndex);
        indices.push_back(i);
        indices.push_back((i + 1) % segments);
    }

    Mesh* coneMesh = new Mesh(name);
    coneMesh->InitFromData(vertices, indices);
    return coneMesh;
}


Mesh* o3D::CreateRoof(const std::string& name, glm::vec3 base, glm::vec3 dimensions, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 halfDim = dimensions / 2.0f;

    vertices = {
        VertexFormat(base + glm::vec3(-halfDim.x, 0.0f, -halfDim.z), color),  // 0: Bottom-left-front
        VertexFormat(base + glm::vec3(halfDim.x, 0.0f, -halfDim.z), color),   // 1: Bottom-right-front
        VertexFormat(base + glm::vec3(halfDim.x, 0.0f, halfDim.z), color),    // 2: Bottom-right-back
        VertexFormat(base + glm::vec3(-halfDim.x, 0.0f, halfDim.z), color),   // 3: Bottom-left-back
        VertexFormat(base + glm::vec3(0.0f, halfDim.y, 0.0f), color)          // 4: Roof apex
    };

    // indicii pentru fetele laterale
    indices = {
        0, 1, 4,  // Front triangle
        1, 2, 4,  // Right triangle
        2, 3, 4,  // Back triangle
        3, 0, 4,  // Left triangle
        0, 1, 2, 2, 3, 0  // Bottom face
    };

    Mesh* roofMesh = new Mesh(name);
    roofMesh->InitFromData(vertices, indices);
    return roofMesh;
}



Mesh* o3D::CreateGate(const std::string& name, float size, float extensionHeight, glm::vec3 outerColor, glm::vec3 innerColor) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Dimensiunile portii
    float halfSize = size / 2.0f;
    float innerFrameOffset = size * 0.1f; // spatiu intre cele 2 componente
    float innerHalfSize = halfSize - innerFrameOffset;
    float extension = extensionHeight;

    // Cea din afara
    vertices.emplace_back(glm::vec3(-halfSize, 0.0f, 0.0f), outerColor);  // Bottom-left
    vertices.emplace_back(glm::vec3(halfSize, 0.0f, 0.0f), outerColor);   // Bottom-right
    vertices.emplace_back(glm::vec3(halfSize, size, 0.0f), outerColor);   // Top-right
    vertices.emplace_back(glm::vec3(-halfSize, size, 0.0f), outerColor);  // Top-left

    // Cea dinauntru
    vertices.emplace_back(glm::vec3(-innerHalfSize, innerFrameOffset, 0.0f), innerColor);  // Bottom-left inner
    vertices.emplace_back(glm::vec3(innerHalfSize, innerFrameOffset, 0.0f), innerColor);   // Bottom-right inner
    vertices.emplace_back(glm::vec3(innerHalfSize, size - innerFrameOffset, 0.0f), innerColor); // Top-right inner
    vertices.emplace_back(glm::vec3(-innerHalfSize, size - innerFrameOffset, 0.0f), innerColor);// Top-left inner

    // barile de jos
    vertices.emplace_back(glm::vec3(-innerHalfSize, -extension, 0.0f), innerColor);
    vertices.emplace_back(glm::vec3(innerHalfSize, -extension, 0.0f), innerColor);

    // indicii pentru linii
    indices = {
        0, 1, 1, 2, 2, 3, 3, 0, // out
        5, 6, 6, 7, 7, 4,       // In
        4, 8, 5, 9              // extra
    };

    Mesh* gateMesh = new Mesh(name);
    gateMesh->SetDrawMode(GL_LINES);
    gateMesh->InitFromData(vertices, indices);
    return gateMesh;
}

Mesh* o3D::CreateArrow(const std::string& name, float shaftLength, float shaftWidth, float headLength, float headWidth, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    float halfShaftWidth = shaftWidth / 2.0f;
    float halfHeadWidth = headWidth / 2.0f;

    // Vertecsi coada
    vertices.emplace_back(glm::vec3(-halfShaftWidth, 0.0f, 0.0f), color); // 0: Bottom-left
    vertices.emplace_back(glm::vec3(halfShaftWidth, 0.0f, 0.0f), color);  // 1: Bottom-right
    vertices.emplace_back(glm::vec3(halfShaftWidth, shaftLength, 0.0f), color);  // 2: Top-right
    vertices.emplace_back(glm::vec3(-halfShaftWidth, shaftLength, 0.0f), color); // 3: Top-left

    // Vertecsi varf
    vertices.emplace_back(glm::vec3(-halfHeadWidth, shaftLength, 0.0f), color);       // 4: Bottom-left
    vertices.emplace_back(glm::vec3(halfHeadWidth, shaftLength, 0.0f), color);        // 5: Bottom-right
    vertices.emplace_back(glm::vec3(0.0f, shaftLength + headLength, 0.0f), color);    // 6: Varf

    // Indicii pentru triunghiuri
    indices = {
        0, 1, 2, 2, 3, 0, // coada
        4, 5, 6           // varf
    };

    Mesh* arrowMesh = new Mesh(name);
    arrowMesh->InitFromData(vertices, indices);
    return arrowMesh;
}