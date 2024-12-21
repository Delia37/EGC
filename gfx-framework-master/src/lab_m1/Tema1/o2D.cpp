#include "lab_m1/Tema1/o2D.h"
#include <vector>
#include <iostream>

Mesh* o2D::CreateTerrain(const std::string& name, const std::vector<int>& heightMap, int pointSpacing, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i < heightMap.size(); ++i) {
        float x = i * pointSpacing;
        float y = heightMap[i];

        // Top vertex
        vertices.emplace_back(glm::vec3(x, y, 0), color);

        // Bottom vertex
        vertices.emplace_back(glm::vec3(x, 0, 0), color);
    }

    // iau indicii pt traiangle strip
    for (int i = 0; i < heightMap.size() - 1; ++i) {
        int topCurrent = i * 2;
        int bottomCurrent = i * 2 + 1;
        int topNext = (i + 1) * 2;
        int bottomNext = (i + 1) * 2 + 1;

        // ii ordonez
        indices.push_back(topCurrent);
        indices.push_back(bottomCurrent);
        indices.push_back(topNext);

        indices.push_back(bottomCurrent);
        indices.push_back(bottomNext);
        indices.push_back(topNext);
    }

    // creez mesh ul
    Mesh* terrainMesh = new Mesh(name);
    terrainMesh->InitFromData(vertices, indices);
    return terrainMesh;
}


Mesh* o2D::CreateUpperTrapezoid(const std::string& name, glm::vec3 color) {
    std::vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(-10.0f, 0.0f, 0), color),
        VertexFormat(glm::vec3(10.0f, 0.0f, 0), color),
        VertexFormat(glm::vec3(6.0f, 6.0f, 0), color),
        VertexFormat(glm::vec3(-6.0f, 6.0f, 0), color)
    };

    std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3 };

    Mesh* lowerTrapezoid = new Mesh(name);
    lowerTrapezoid->InitFromData(vertices, indices);
    return lowerTrapezoid;
}

Mesh* o2D::CreateLowerTrapezoid(const std::string& name, glm::vec3 color) {
    std::vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(-8.0f, 0.0f, 0), color),
        VertexFormat(glm::vec3(8.0f, 0.0f, 0), color),
        VertexFormat(glm::vec3(4.0f, -5.0f, 0), color),
        VertexFormat(glm::vec3(-4.0f, -5.0f, 0), color)
    };

    std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3 };

    Mesh* upperTrapezoid = new Mesh(name);
    upperTrapezoid->InitFromData(vertices, indices);
    return upperTrapezoid;
}



Mesh* o2D::CreateTankTurret(const std::string& name, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    int numSegments = 20;
    float radius = 0.3f;

    // vertexul central
    vertices.emplace_back(glm::vec3(0, 0, 0), color);

    for (int i = 0; i <= numSegments; ++i) {
        float angle = M_PI * (i / static_cast<float>(numSegments));
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        vertices.emplace_back(glm::vec3(x, y, 0), color);
        indices.push_back(i);
    }

    Mesh* turretMesh = new Mesh(name);
    turretMesh->SetDrawMode(GL_TRIANGLE_FAN);
    turretMesh->InitFromData(vertices, indices);
    return turretMesh;
}

Mesh* o2D::CreateTankBarrel(const std::string& name, glm::vec3 color) {
    std::vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(-0.05f, 0.0f, 0), color),
        VertexFormat(glm::vec3(0.05f, 0.0f, 0), color),
        VertexFormat(glm::vec3(0.05f, 0.5f, 0), color),
        VertexFormat(glm::vec3(-0.05f, 0.5f, 0), color)
    };
    std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3 };

    Mesh* barrelMesh = new Mesh(name);
    barrelMesh->InitFromData(vertices, indices);
    return barrelMesh;
}

Mesh* o2D::CreateTrajectoryMesh(const std::vector<glm::vec2>& trajectoryPoints) {
    std::vector<VertexFormat> vertices;

    for (const auto& point : trajectoryPoints) {
        vertices.emplace_back(glm::vec3(point.x, point.y, 0), glm::vec3(1.0f, 1.0f, 1.0f));
    }
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < trajectoryPoints.size(); ++i) {
        indices.push_back(i); // indicii pentru a forma o linie
    }

    Mesh* trajectoryMesh = new Mesh("trajectory");
    trajectoryMesh->SetDrawMode(GL_LINE_STRIP);
    trajectoryMesh->InitFromData(vertices, indices);

    return trajectoryMesh;
}

Mesh* o2D::CreateProjectile(const std::string& name, float radius, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    int numSegments = 30;

    vertices.emplace_back(glm::vec3(0, 0, 0), color);

    for (int i = 0; i <= numSegments; ++i) {
        float angle = 2 * M_PI * (i / static_cast<float>(numSegments));
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        vertices.emplace_back(glm::vec3(x, y, 0), color);
        indices.push_back(i);
    }

    Mesh* projectileMesh = new Mesh(name);
    projectileMesh->SetDrawMode(GL_TRIANGLE_FAN);
    projectileMesh->InitFromData(vertices, indices);
    return projectileMesh;
}

Mesh* o2D::CreateRectangle(const std::string& name, glm::vec3 corner, float width, float height, glm::vec3 color, bool fill) {
    std::vector<VertexFormat> vertices = {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    Mesh* rectangleMesh = new Mesh(name);
    if (!fill) {
        // pt contur
        rectangleMesh->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // filled 
        indices.push_back(0);
        indices.push_back(2);
        rectangleMesh->SetDrawMode(GL_TRIANGLE_FAN);
    }

    rectangleMesh->InitFromData(vertices, indices);
    return rectangleMesh;
}
