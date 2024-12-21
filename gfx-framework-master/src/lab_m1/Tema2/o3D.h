#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace o3D
{
    Mesh* CreateTerrain(const std::string& name, int rows, int cols, float spacing, glm::vec3 color);
    Mesh* CreateCube(const std::string& name, glm::vec3 center, float sideLength, glm::vec3 color);
    Mesh* CreateParallelepiped(const std::string& name, glm::vec3 center, glm::vec3 dimensions, glm::vec3 color);
    Mesh* CreateOutlineParallelepiped(const std::string& name, glm::vec3 center, glm::vec3 dimensions, glm::vec3 color);
    Mesh* CreateCylinder(const std::string& name, int segments, float radius, float height, glm::vec3 color);
    Mesh* CreateCone(const std::string& name, int segments, float radius, float height, glm::vec3 color);
    Mesh* CreateRoof(const std::string& name, glm::vec3 base, glm::vec3 dimensions, glm::vec3 color);
    Mesh* CreateGate(const std::string& name, float size, float extensionHeight, glm::vec3 outerColor, glm::vec3 innerColor);
    Mesh* CreateArrow(const std::string& name, float shaftLength, float shaftWidth, float headLength, float headWidth, glm::vec3 color);
}

