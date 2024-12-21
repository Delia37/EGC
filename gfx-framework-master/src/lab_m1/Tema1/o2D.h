#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace o2D
{
    Mesh* CreateTerrain(const std::string& name, const std::vector<int>& heightMap, int pointSpacing, glm::vec3 color);
    Mesh* CreateTankTurret(const std::string& name, glm::vec3 color);
    Mesh* CreateTankBarrel(const std::string& name, glm::vec3 color);
    Mesh* CreateUpperTrapezoid(const std::string& name, glm::vec3 color);
    Mesh* CreateLowerTrapezoid(const std::string& name, glm::vec3 color);
    Mesh* CreateTrajectoryMesh(const std::vector<glm::vec2>& trajectoryPoints);
    Mesh* CreateProjectile(const std::string& name, float radius, glm::vec3 color);
    Mesh* CreateRectangle(const std::string& name, glm::vec3 corner, float width, float height, glm::vec3 color, bool fill);
}

