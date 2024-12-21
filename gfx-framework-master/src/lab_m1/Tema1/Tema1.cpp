#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <GL/glew.h>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/o2D.h"

using namespace std;
using namespace m1;

const float maxHealth = 100.0f;

Tema1::Tema1() {}

Tema1::~Tema1() {}

void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    int terrainWidth = 1285;         // width teren in pixeli
    pointSpacing = 10;               // spatierea dintre puncte

    tankX1 = 500.0f;                 // x initial
    tankX2 = 800.0f;
    tankSpeed = 100.0f;              // viteza

    tankAngle1 = 0.0f;
    tankAngle2 = 0.0f;

    barrelAngle1 = 0.0f;
    barrelAngle2 = 0.0f;

    tank1Health = maxHealth;
    tank2Health = maxHealth;

    glm::vec3 terrainColor(0.5f, 0.35f, 0.05f); // maro pentru teren

    int baseHeight = 250;

    // calcul heightMap
    for (int i = 0; i < terrainWidth / pointSpacing; ++i) {
        float x = i * pointSpacing;
        int height = static_cast<int>(
            baseHeight +
            80 * sin(0.01 * x) +
            40 * sin(0.005 * x) +
            50 * sin(0.03 * x)
            );
        heightMap.push_back(height);
    }

    // creez mesh pt teren
    Mesh* terrainMesh = o2D::CreateTerrain("terrain", heightMap, pointSpacing, terrainColor);
    AddMeshToList(terrainMesh);

    // Tank1 - culori
    glm::vec3 turretColor1(0.8f, 0.6f, 0.4f); // maro deschis
    glm::vec3 barrelColor1(0.1f, 0.1f, 0.1f); // negru
    glm::vec3 upperColor1(0.8f, 0.6f, 0.4f);  // maro deschis pt sus
    glm::vec3 lowerColor1(0.3f, 0.2f, 0.1f);  // maro inchis pt jos

    // Tank2 - culori
    glm::vec3 turretColor2(0.2f, 0.6f, 0.2f); // verde deschis
    glm::vec3 barrelColor2(0.1f, 0.1f, 0.1f); // negru
    glm::vec3 upperColor2(0.2f, 0.6f, 0.2f);  // verde deschis
    glm::vec3 lowerColor2(0.1f, 0.5f, 0.1f);  // verde negru

    // creez mesh-urile pentru primul tank
    Mesh* lowerTrapezoid1 = o2D::CreateLowerTrapezoid("tank1_lower_trapezoid", lowerColor1);
    AddMeshToList(lowerTrapezoid1);

    Mesh* upperTrapezoid1 = o2D::CreateUpperTrapezoid("tank1_upper_trapezoid", upperColor1);
    AddMeshToList(upperTrapezoid1);

    Mesh* tankTurret1 = o2D::CreateTankTurret("tank1_turret", turretColor1);
    AddMeshToList(tankTurret1);

    Mesh* tankBarrel1 = o2D::CreateTankBarrel("tank1_barrel", barrelColor1);
    AddMeshToList(tankBarrel1);

    // mesh-urile pentru al doilea tank
    Mesh* lowerTrapezoid2 = o2D::CreateLowerTrapezoid("tank2_lower_trapezoid", lowerColor2);
    AddMeshToList(lowerTrapezoid2);

    Mesh* upperTrapezoid2 = o2D::CreateUpperTrapezoid("tank2_upper_trapezoid", upperColor2);
    AddMeshToList(upperTrapezoid2);

    Mesh* tankTurret2 = o2D::CreateTankTurret("tank2_turret", turretColor2);
    AddMeshToList(tankTurret2);

    Mesh* tankBarrel2 = o2D::CreateTankBarrel("tank2_barrel", barrelColor2);
    AddMeshToList(tankBarrel2);

    Mesh* projectileMesh = o2D::CreateProjectile("projectile", 5.0f, glm::vec3(1, 0, 0)); // rosu pt proictil
    AddMeshToList(projectileMesh);

}

void Tema1::Update(float deltaTimeSeconds)
{
    modelMatrix = glm::mat3(1);
    RenderMesh(meshes["terrain"], shaders["VertexColor"], modelMatrix);

    //verifica constant daca exista alunecari de teren
    ApplyLandslideAnimation(deltaTimeSeconds);

    // daca nu a scazut bara de viata
    if (tank1Health > 0) {
        RenderTank(tankX1, tankAngle1, "tank1", 26.0f);

        //generez traiectoia din varful tunului
        std::vector<glm::vec2> trajectoryPoints1 = GenerateTrajectoryPoints(barrelAngle1, 50.0f, barrelTipPosition1.y, 9.8f);

        //o fac sa se opreasca in teren
        std::vector<glm::vec2> visibleTrajectoryPoints1 = GetVisibleTrajectoryPoints(trajectoryPoints1,
            barrelTipPosition1.x, heightMap);

        Mesh* trajectoryMesh1 = o2D::CreateTrajectoryMesh(visibleTrajectoryPoints1);
        glm::mat3 trajectoryTransform1 = glm::mat3(1.0f);
        RenderMesh(trajectoryMesh1, shaders["VertexColor"], trajectoryTransform1);
    }

    // la fel si pentru tank 2
    if (tank2Health > 0) {
        RenderTank(tankX2, tankAngle2, "tank2", 26.0f);

        std::vector<glm::vec2> trajectoryPoints2 = GenerateTrajectoryPoints(barrelAngle2, 50.0f,
            barrelTipPosition2.y, 9.8f);

        std::vector<glm::vec2> visibleTrajectoryPoints2 = GetVisibleTrajectoryPoints(trajectoryPoints2,
            barrelTipPosition2.x, heightMap);

        Mesh* trajectoryMesh2 = o2D::CreateTrajectoryMesh(visibleTrajectoryPoints2);
        glm::mat3 trajectoryTransform2 = glm::mat3(1.0f);
        RenderMesh(trajectoryMesh2, shaders["VertexColor"], trajectoryTransform2);
    }

    // Update pt proiectilele lansate de tank 1
    if (tank1Health > 0) {
        for (size_t i = 0; i < projectiles1.size(); ++i) {
            UpdateProjectile(projectiles1[i], deltaTimeSeconds);

            // sterg proiectilul daca nu mai este activ
            if (!projectiles1[i].active) {
                projectiles1.erase(projectiles1.begin() + i);
                --i;
            }
        }
    }

    // la fel si pt tank2
    if (tank2Health > 0) {
        for (size_t i = 0; i < projectiles2.size(); ++i) {
            UpdateProjectile(projectiles2[i], deltaTimeSeconds);

            if (!projectiles2[i].active) {
                projectiles2.erase(projectiles2.begin() + i);
                --i;
            }
        }
    }
}

void Tema1::FrameStart()
{
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);  // cerul
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::RenderTank(float tankX, float tankAngle, const std::string& tankPrefix, float yOffset)
{
    // ieu indexul din heightMap in functie de pozitia tancului
    int index = static_cast<int>(tankX / pointSpacing);

    // ma asigur ca se afla in heightMap
    if (index >= 0 && index < heightMap.size() - 1) {
        glm::vec2 A(index * pointSpacing, heightMap[index]);
        glm::vec2 B((index + 1) * pointSpacing, heightMap[index + 1]);

        // iau y in functie de x
        float t = (tankX - A.x) / (B.x - A.x);
        float tankY = A.y + t * (B.y - A.y);

        // unghiul care aliniaza tancul cu terenul
        glm::vec2 V = B - A;
        tankAngle = atan2(V.y, V.x);

        // il translatez pe teteren si il rotesc in functie de heightMap
        glm::mat3 tankTransform = transform2D::Translate(tankX, tankY + yOffset + 5.0f) * transform2D::Rotate(tankAngle);

        // pun fiecare forma una peste alta folosind pozitia calculata mai sus

        glm::mat3 lowerScale = transform2D::Scale(6.0f, 2.5f);
        glm::mat3 lowerTransform = tankTransform * lowerScale;
        RenderMesh2D(meshes[tankPrefix + "_lower_trapezoid"], shaders["VertexColor"], lowerTransform);

        glm::mat3 upperScale = transform2D::Scale(5.5f, 2.2f);
        glm::mat3 upperTransform = tankTransform * upperScale;
        RenderMesh2D(meshes[tankPrefix + "_upper_trapezoid"], shaders["VertexColor"], upperTransform);


        glm::mat3 turretPosition = transform2D::Translate(0, 25.0f);
        glm::mat3 turretScale = transform2D::Scale(65.0f, 65.0f);
        glm::mat3 turretTransform = tankTransform * turretPosition * turretScale;
        RenderMesh2D(meshes[tankPrefix + "_turret"], shaders["VertexColor"], turretTransform);

        float barrelAngle = (tankPrefix == "tank1") ? barrelAngle1 : barrelAngle2;
        glm::mat3 barrelPosition = transform2D::Translate(0.0f, 0.2f);
        glm::mat3 barrelRotation = transform2D::Rotate(barrelAngle);     // tunul se roteste in functie de alt unghi
        glm::mat3 barrelScale = transform2D::Scale(1.0f, 0.5f);
        glm::mat3 barrelTransform = turretTransform * barrelPosition * barrelRotation * barrelScale;
        RenderMesh2D(meshes[tankPrefix + "_barrel"], shaders["VertexColor"], barrelTransform);

        glm::vec2 barrelTipLocal = glm::vec2(0.0f, 0.5f);
        glm::vec2 barrelTipWorld = barrelTransform * glm::vec3(barrelTipLocal, 1.0f);

        // am nevoie de coordonata varfului tunului la traiectorie
        if (tankPrefix == "tank1") {
            barrelTipPosition1 = barrelTipWorld;
            barrelAngle1 = barrelAngle1;
        }
        else {
            barrelTipPosition2 = barrelTipWorld;
            barrelAngle2 = barrelAngle2;
        }

        // bara de viata
        float healthBarYOffset = 70.0f; // distanta deasupra tancului
        float healthBarWidth = 50.0f;
        float healthBarHeight = 10.0f;

        float currentHealth = (tankPrefix == "tank1") ? tank1Health : tank2Health;
        float healthPercentage = currentHealth / maxHealth;

        // dreptunghiul de contur
        glm::mat3 healthBackgroundTransform = transform2D::Translate(tankX - healthBarWidth / 2, tankY + yOffset + healthBarYOffset + 10.0f);
        RenderMesh2D(o2D::CreateRectangle("health_background", glm::vec3(0, 0, 0), healthBarWidth, healthBarHeight,
            glm::vec3(0, 1, 0), false), shaders["VertexColor"], healthBackgroundTransform);

        // derptunghiul filled - cel dinamic
        glm::mat3 healthForegroundTransform = transform2D::Translate(tankX - healthBarWidth / 2, tankY + yOffset + healthBarYOffset + 10.0f);
        RenderMesh2D(o2D::CreateRectangle("health_foreground", glm::vec3(0, 0, 0), healthBarWidth * healthPercentage, healthBarHeight,
            glm::vec3(1, 1, 1), true), shaders["VertexColor"], healthForegroundTransform);
    }
}


std::vector<glm::vec2> Tema1::GenerateTrajectoryPoints(float angle, float magnitude, float initialY, float g) {

    std::vector<glm::vec2> points;
    float angleT = angle + glm::pi<float>() / 2;
    magnitude += 20.0f;

    float vX = magnitude * cos(angleT);
    float vY = magnitude * sin(angleT);

    //pt a face traiectoria sa se incline stanga/dreapta
    vX *= (angleT >= 0.0f && angleT <= 90.0f) ? 1.0f : -1.0f;

    float t = 0.0f;
    float timeStep = 0.05f;

    // generez puncte pana ce proiectilul atinge pamantul
    while (true) {
        // calculez x si y in functie de timpul t
        float x = vX * t;
        float y = initialY + vY * t - 0.5f * g * t * t;

        if (y < 0) break;
        points.push_back(glm::vec2(x, y));

        t += timeStep;
    }

    return points;
}



std::vector<glm::vec2> Tema1::GetVisibleTrajectoryPoints(const std::vector<glm::vec2>& trajectoryPoints,
    float barrelTipX, const std::vector<int>& heightMap) {
    std::vector<glm::vec2> visibleTrajectoryPoints;

    for (auto& point : trajectoryPoints) {
        glm::vec2 worldPoint = point + glm::vec2(barrelTipX, 0);

        int index = static_cast<int>(worldPoint.x / pointSpacing);

        // ma opresc daca ies din heightMap
        if (index < 0 || index >= heightMap.size() - 1) {
            break;
        }

        float x1 = index * pointSpacing;
        float x2 = (index + 1) * pointSpacing;
        float y1 = heightMap[index];
        float y2 = heightMap[index + 1];

        float t = (worldPoint.x - x1) / (x2 - x1);
        float interpolatedY = y1 * (1 - t) + y2 * t;

        // daca punctul e deasupra, il fac vizibil
        if (worldPoint.y > interpolatedY) {
            visibleTrajectoryPoints.push_back(worldPoint);
        }
        else {
            break;
        }
    }

    return visibleTrajectoryPoints;
}


void Tema1::UpdateProjectile(Projectile& projectile, float deltaTime) {
    const float projectileRadius = 10.0f;
    const float explosionRadius = 40.0f;
    const float tankRadius = 45.0f;
    const float collisionThreshold = 5.0f; // marja de eroare pt coliziune

    if (projectile.active && projectile.currentPoint < projectile.trajectory.size()) {
        projectile.position = projectile.trajectory[projectile.currentPoint];
        projectile.currentPoint++;

        glm::mat3 modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(projectile.position.x, projectile.position.y);
        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);

        // vad daca am coliziune cu terenul
        int terrainIndex = static_cast<int>(projectile.position.x / pointSpacing);
        if (terrainIndex >= 0 && terrainIndex < heightMap.size() - 1) {
            float x1 = terrainIndex * pointSpacing;
            float y1 = heightMap[terrainIndex];
            float x2 = (terrainIndex + 1) * pointSpacing;
            float y2 = heightMap[terrainIndex + 1];

            // y in functie de x
            float t = (projectile.position.x - x1) / (x2 - x1);
            float interpolatedY = y1 * (1 - t) + y2 * t;

            // vad daca e sub pragul stabilit
            if (projectile.position.y <= interpolatedY + collisionThreshold) {
                DeformTerrain(projectile.position.x, explosionRadius);
                projectile.active = false; // il deactivez dupa coliziune
                return;
            }

            // verific coliziunea cu tancurile

            float distanceToTank1 = glm::distance(projectile.position,
                glm::vec2(tankX1, heightMap[static_cast<int>(tankX1 / pointSpacing)] + 26.0f));

            if (distanceToTank1 <= projectileRadius + tankRadius) {
                tank1Health -= 10.0f; // scad bara de viata
                projectile.active = false;
                return;
            }

            float distanceToTank2 = glm::distance(projectile.position,
                glm::vec2(tankX2, heightMap[static_cast<int>(tankX2 / pointSpacing)] + 26.0f));

            if (distanceToTank2 <= projectileRadius + tankRadius) {
                tank2Health -= 10.0f;
                projectile.active = false;
                return;
            }
        }

        // daca a ajuns la sfarsitul traiectoriei
        if (projectile.currentPoint >= projectile.trajectory.size()) {
            projectile.active = false;
        }
    }
}

void Tema1::DeformTerrain(float explosionX, float radius) {
    int centerIndex = static_cast<int>(explosionX / pointSpacing);
    int radiusPoints = static_cast<int>(radius / pointSpacing);

    // pastrez vectorul curent
    std::vector<int> previousHeightMap = heightMap;

    // iterez prin punctele din raza exploziei
    for (int i = -radiusPoints; i <= radiusPoints; ++i) {
        int index = centerIndex + i;

        // daca e in heightMap
        if (index >= 0 && index < heightMap.size()) {
            // distanta orizontala de la centru
            float distanceX = i * pointSpacing;

            // vad daca e in raza
            if (std::abs(distanceX) <= radius) {
                // adancimea deformarii
                float deformationDepth = std::sqrt(radius * radius - distanceX * distanceX);
                float targetHeight = previousHeightMap[centerIndex] - deformationDepth;
                heightMap[index] = std::max(0.0f, targetHeight);
            }
        }
    }

    // recalculez terenul cu noul heightMap
    Mesh* updatedTerrainMesh = o2D::CreateTerrain("terrain", heightMap, pointSpacing, glm::vec3(0.5f, 0.35f, 0.05f));
    meshes["terrain"] = updatedTerrainMesh;
}


void Tema1::ApplyLandslideAnimation(float deltaTime) {
    float threshold = 20.0f;  // diferenta de inaltime
    float transferRate = 5.0f; // Rate of height transfer, can be adjusted

    for (int i = 0; i < heightMap.size() - 1; ++i) {
        // diferenta de inaltime intre puncte adiacente
        float d = std::abs(heightMap[i] - heightMap[i + 1]);

        // daca depaseste pragul
        if (d > threshold) {
            float epsilon = transferRate * deltaTime;

            // iau inaltime de sus -> jos
            if (heightMap[i] > heightMap[i + 1]) {
                heightMap[i] -= epsilon;
                heightMap[i + 1] += epsilon;
            }
            else {
                heightMap[i] += epsilon;
                heightMap[i + 1] -= epsilon;
            }
        }
    }

    // recalculez terenul
    Mesh* updatedTerrainMesh = o2D::CreateTerrain("terrain", heightMap, pointSpacing, glm::vec3(0.5f, 0.35f, 0.05f));
    meshes["terrain"] = updatedTerrainMesh;
}


void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_D)) {
        tankX1 += tankSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        tankX1 -= tankSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        tankX2 += tankSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_LEFT)) {
        tankX2 -= tankSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_W)) {
        barrelAngle1 += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        barrelAngle1 -= deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_UP)) {
        barrelAngle2 += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_DOWN)) {
        barrelAngle2 -= deltaTime;
    }

    // limitez miscarea la +/- 90 de grade
    barrelAngle1 = glm::clamp(barrelAngle1, -glm::pi<float>() / 2, glm::pi<float>() / 2);
    barrelAngle2 = glm::clamp(barrelAngle2, -glm::pi<float>() / 2, glm::pi<float>() / 2);
}

void Tema1::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE) {
        Projectile newProjectile;
        newProjectile.trajectory = GenerateTrajectoryPoints(barrelAngle1, 50.0f, barrelTipPosition1.y, 9.8f);

        // pun punctele din varful tunului
        for (size_t i = 0; i < newProjectile.trajectory.size(); ++i) {
            newProjectile.trajectory[i].x += barrelTipPosition1.x;
            if (i == 0) {
                newProjectile.trajectory[i].y = barrelTipPosition1.y;
            }
        }

        newProjectile.position = barrelTipPosition1;
        newProjectile.active = true;
        newProjectile.currentPoint = 0;
        projectiles1.push_back(newProjectile);  // un nou proiectil in vector
    }

    if (key == GLFW_KEY_ENTER) {
        Projectile newProjectile;
        newProjectile.trajectory = GenerateTrajectoryPoints(barrelAngle2, 50.0f, barrelTipPosition2.y, 9.8f);

        for (size_t i = 0; i < newProjectile.trajectory.size(); ++i) {
            newProjectile.trajectory[i].x += barrelTipPosition2.x;
            if (i == 0) {
                newProjectile.trajectory[i].y = barrelTipPosition2.y;
            }
        }

        newProjectile.position = barrelTipPosition2;
        newProjectile.active = true;
        newProjectile.currentPoint = 0;
        projectiles2.push_back(newProjectile);
    }
}



void Tema1::OnKeyRelease(int key, int mods)
{
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
