#include "lab_m1/Tema2/Tema2.h"
#include <vector>
#include <string>
#include <iostream>
#include "lab_m1/Tema2/transform3D.h"
#include "lab_m1/Tema2/o3D.h"


using namespace std;
using namespace m1;

float maxTime = 120.0f;  // Durata jocului in seconde
float remainingTime = maxTime;  // Timpul ramas pana la terminarea jocului
std::vector<std::string> gateMeshes;  // Retin numele mesh-urilor pentru fiecare poartã

Tema2::Tema2() {}

Tema2::~Tema2() {}

void Tema2::Init()
{
    glm::ivec2 resolution = window->GetResolution();

    // Initializez TextRenderer
    textRenderer = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);

    // Incarc fontul pentru text
    textRenderer->Load("assets/fonts/Hack-Bold.ttf", 48);

    dronePosition = glm::vec3(0.0f, 10.0f, 0.0f); // Pozitia initiala a dronei
    droneRotation = 0.0f; // Initial drona nu e rotita
    moveSpeed = 15.0f;
    rotationSpeed = glm::radians(45.0f); //viteza de rotatie in radini pe secunda
    currentGateIndex = 0;
    isGameOver = false;
    isSuccess = false;

    // Get resolution and setup orthographic camera
    auto camera = GetSceneCamera();
    camera->SetPerspective(60.0f, window->props.aspectRatio, 0.01f, 400.0f);
    camera->SetPosition(glm::vec3(0.0f, 10.0f, -20.0f)); // Align behind the drone
    camera->Update();
    GetCameraInput()->SetActive(false);

    // Initializez shaders
    Shader* shader = new Shader("TerrainShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "vertex_shader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "fragment_shader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;


    // Initializez terenul
    Mesh* terrain = o3D::CreateTerrain("terrain", 100, 100, 1.0f, glm::vec3(0.5f, 0.5f, 0.5f));
    AddMeshToList(terrain);

    // Latura unui cub folosit pentru constructia dronei
    float cubeSide = 0.3f;

    // Corpul dronei
    float bodyLength = 2.0f, bodyWidth = cubeSide, bodyHeight = cubeSide;
    Mesh* body = o3D::CreateParallelepiped("body", glm::vec3(0, 0, 0), glm::vec3(bodyLength, bodyHeight, bodyWidth), glm::vec3(0.7f, 0.7f, 0.7f));
    AddMeshToList(body);

    // Cuburile de pe corpul dronei
    Mesh* cube = o3D::CreateCube("cube", glm::vec3(0, 0, 0), cubeSide, glm::vec3(0.7f, 0.7f, 0.7f));
    AddMeshToList(cube);

    // Elicele deasupra cuburilor
    Mesh* propeller = o3D::CreateParallelepiped("propeller", glm::vec3(0, 0, 0), glm::vec3(1.0f, 0.1f, 0.1f), glm::vec3(0.0f, 0.0f, 0.0f));
    AddMeshToList(propeller);

    // Trunchiul copacului
    Mesh* trunk = o3D::CreateCylinder("trunk", 20, 0.1f, 1.0f, glm::vec3(0.6f, 0.3f, 0.0f));  // Brown trunk
    AddMeshToList(trunk);

    // Frunzele copacului
    Mesh* foliage = o3D::CreateCone("foliage", 20, 0.5f, 1.0f, glm::vec3(0.0f, 0.6f, 0.0f));  // Green foliage
    AddMeshToList(foliage);

    // Iau dimensiunile terenului pentru a pune obstacolele pe toata suprafata lui
    float terrainStartX = -50.0f;
    float terrainEndX = 150.0f;
    float terrainStartZ = -50.0f;
    float terrainEndZ = 150.0f;

    for (int i = 0; i < 70; i++) {
        float x = terrainStartX + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (terrainEndX - terrainStartX)));
        float z = terrainStartZ + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (terrainEndZ - terrainStartZ)));
        float scale = 1.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 1.5f)); // dimensiuni variabile
        treePositions.push_back(glm::vec3(x, 1.0f, z));  // Ii plasez pe teren
        treeScales.push_back(scale);
    }

    // Componentele pentru casa
    Mesh* houseBase = o3D::CreateParallelepiped("houseBase", glm::vec3(0, 0, 0), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.8f, 0.5f, 0.3f));
    AddMeshToList(houseBase);

    Mesh* roof = o3D::CreateRoof("roof", glm::vec3(0, 0, 0), glm::vec3(2.5f, 1.0f, 2.5f), glm::vec3(0.9f, 0.1f, 0.1f));
    AddMeshToList(roof);

    // Distribui casele random
    for (int i = 0; i < 15; i++) {
        float x = terrainStartX + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (terrainEndX - terrainStartX)));
        float z = terrainStartZ + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (terrainEndZ - terrainStartZ)));
        float scale = 1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 1.0f));
        housePositions.push_back(glm::vec3(x, 1.0f, z));
        houseScales.push_back(scale);
    }

    // Initializez portile
    InitializeGates(terrainStartX, terrainEndX, terrainStartZ, terrainEndZ);

    // Bara de viata
    Mesh* lifeBar = o3D::CreateParallelepiped("lifeBar", glm::vec3(0, 0, 0), glm::vec3(0.5f, 5.0f, 0.1f), glm::vec3(1.0f, 1.0f, 0.0f));
    AddMeshToList(lifeBar);

    Mesh* lifeBarOutline = o3D::CreateOutlineParallelepiped("lifeBarOutline", glm::vec3(0, 0, 0), glm::vec3(0.52f, 5.0f, 0.1f), glm::vec3(0.0f, 0.0f, 0.0f));
    AddMeshToList(lifeBarOutline);
}

void Tema2::InitializeGates(float terrainStartX, float terrainEndX, float terrainStartZ, float terrainEndZ) {
    // Cele 2 mesh uri
    Mesh* greenGate = o3D::CreateGate("greenGate", 4.0f, 2.75f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    AddMeshToList(greenGate);

    Mesh* redGate = o3D::CreateGate("redGate", 4.0f, 2.75f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    AddMeshToList(redGate);

    Mesh* arrow = o3D::CreateArrow("arrow", 2.0f, 0.2f, 1.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    AddMeshToList(arrow);

    float gateWidth = 4.0f;
    float gateHeight = 5.0f;

    int gateCount = 10; // Numarul de porti
    for (int i = 0; i < gateCount; i++) {
        glm::vec3 gatePosition;
        bool isValidPosition = false;

        // Generez o pozitie valida
        while (!isValidPosition) {
            gatePosition = GetRandomPosition(terrainStartX, terrainEndX, terrainStartZ,
                terrainEndZ, 4.0f); // O pozitie deasupra terenului
            isValidPosition = true;

            // Coliziune cu brazii
            for (size_t j = 0; j < treePositions.size(); ++j) {
                float treeScale = treeScales[j];
                if (CheckSphereCylinderCollision(gatePosition, gateWidth / 2, treePositions[j],
                    treePositions[j] + glm::vec3(0, 1.2f * treeScale, 0), 0.4f * treeScale)) {
                    isValidPosition = false;
                    break;
                }
            }

            // Coliziune cu casele
            for (size_t j = 0; j < housePositions.size(); ++j) {
                float houseScale = houseScales[j];
                glm::vec3 houseMin = housePositions[j] - glm::vec3(houseScale + 4.0f, 0.0f, houseScale + 3.0f);
                glm::vec3 houseMax = housePositions[j] + glm::vec3(houseScale + 4.0f, 2.0f * houseScale, houseScale + 3.0f);

                if (CheckSphereAABB(gatePosition, gateWidth / 2, houseMin, houseMax)) {
                    isValidPosition = false;
                    break;
                }
            }
        }

        // Adaug pozitia valida in vector
        gatePositions.push_back(gatePosition);
        gateScales.push_back(1.0f); // Uniform scale for gates
    }

    // Iau o ordine de parcurgere
    for (int i = 0; i < gatePositions.size(); ++i) {
        gateTraversalOrder.push_back(i);
        gateColors.push_back(glm::vec3(0.0f, 1.0f, 0.0f)); // initial toate sunt verzi
    }

    std::random_shuffle(gateTraversalOrder.begin(), gateTraversalOrder.end());

    for (int i = 0; i < gatePositions.size(); ++i) {
        if (i == gateTraversalOrder[0]) {
            gateMeshes.push_back("redGate"); // Prima va fi rosie
        }
        else {
            gateMeshes.push_back("greenGate"); // Restul verzi
        }
    }
}

glm::vec3 Tema2::GetRandomPosition(float terrainStartX, float terrainEndX, float terrainStartZ, float terrainEndZ, float height) {
    float x = terrainStartX + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (terrainEndX - terrainStartX)));
    float z = terrainStartZ + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (terrainEndZ - terrainStartZ)));
    return glm::vec3(x, height, z);
}


void Tema2::Update(float deltaTimeSeconds)
{
    if (remainingTime > 0) {
        remainingTime -= deltaTimeSeconds;
        if (remainingTime <= 0) {
            remainingTime = 0;
            isGameOver = true;  // S-a terminat timpul
        }
    }

    // Updatez pozitia camerei
    UpdateCamera();

    RenderTerrain(glm::vec3(-50.0f, 0.0f, -50.0f), glm::vec3(2.0f, 1.0f, 2.0f));
    RenderTrees();
    RenderHouses();
    RenderDrone(dronePosition, droneRotation, glm::vec3(2.0f, 0.3f, 0.3f), glm::vec3(1.0f, 0.1f, 0.1f), 5.0f);
    RenderGates();

    if (!isSuccess && !isGameOver && currentGateIndex < gateTraversalOrder.size()) {
        if (CheckDroneInGate(dronePosition, gatePositions[gateTraversalOrder[currentGateIndex]])) {
            // Parta prin care am trecut devine verde
            SetGateToGreen(gateTraversalOrder[currentGateIndex]);

            // Trec la urmatoarea
            currentGateIndex++;

            // Verific daca am trecut prin toate
            if (currentGateIndex >= gateTraversalOrder.size()) {
                isSuccess = true;
            }
            else {
                // Urmatoarea devine cea rosie
                SetGateToRed(gateTraversalOrder[currentGateIndex]);
            }
        }


    }

    // Pun sageata si hp-ul daca au mai ramas porti de parcurs
    if (!isSuccess && currentGateIndex < gateTraversalOrder.size()) {
        RenderArrowToNextGate(dronePosition);
        RenderLifeBar(dronePosition);
    }

    //RenderLifeBar(dronePosition);

    if (isGameOver || isSuccess)
    {
        RenderGameOverText(); // "GAME OVER"
        return; // Nu mai fac alte actualizari
    }

}

void Tema2::RenderTerrain(glm::vec3 position, glm::vec3 scale)
{
    //Folosesc shaderul pentru teren
    Shader* shader = shaders["TerrainShader"];
    shader->Use();

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = transform3D::Translate(position.x, position.y, position.z);
    modelMatrix *= transform3D::Scale(scale.x, scale.y, scale.z);

    // Trimit catre shader
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "view"), 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "projection"), 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));

    RenderMesh(meshes["terrain"], shader, modelMatrix);
}

void Tema2::RenderTrees() {
    Shader* shader = shaders["VertexColor"];
    shader->Use();

    for (int i = 0; i < treePositions.size(); i++) {
        glm::vec3 position = treePositions[i];
        float scale = treeScales[i];

        // Trunchiul
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = transform3D::Translate(position.x, position.y, position.z);
        modelMatrix *= transform3D::Scale(0.4f * scale, 1.2f * scale, 0.4f * scale);
        RenderMesh(meshes["trunk"], shader, modelMatrix);

        // Primul con
        modelMatrix = glm::mat4(1);
        modelMatrix = transform3D::Translate(position.x, position.y + 1.0f * scale, position.z);
        modelMatrix *= transform3D::Scale(0.8f * scale, 1.2f * scale, 0.8f * scale);
        RenderMesh(meshes["foliage"], shader, modelMatrix);

        // Al doilea con deasupra
        modelMatrix = glm::mat4(1);
        modelMatrix = transform3D::Translate(position.x, position.y + 2.0f * scale, position.z);
        modelMatrix *= transform3D::Scale(0.6f * scale, 1.0f * scale, 0.6f * scale);
        RenderMesh(meshes["foliage"], shader, modelMatrix);
    }
}

void Tema2::RenderHouses() {
    Shader* shader = shaders["VertexColor"];
    shader->Use();

    for (int i = 0; i < housePositions.size(); i++) {
        glm::vec3 position = housePositions[i];
        float scale = houseScales[i] * 2.0f;

        // Deasupra terenului
        float baseHeightOffset = 2.0f;
        float roofHeightOffset = scale;

        // Baza casei
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = transform3D::Translate(position.x, position.y + baseHeightOffset, position.z);
        modelMatrix *= transform3D::Scale(scale, scale, scale);
        RenderMesh(meshes["houseBase"], shader, modelMatrix);

        // Acoperisul
        modelMatrix = glm::mat4(1);
        modelMatrix = transform3D::Translate(position.x, position.y + baseHeightOffset + roofHeightOffset, position.z);
        modelMatrix *= transform3D::Scale(scale, scale, scale);
        RenderMesh(meshes["roof"], shader, modelMatrix);
    }
}

void Tema2::RenderDrone(glm::vec3 position, float rotation, glm::vec3 bodyScale, glm::vec3 propellerScale, float rotationSpeed)
{
    Shader* shader = shaders["VertexColor"];
    shader->Use();

    // Rotatia elicei
    float angle = Engine::GetElapsedTime() * rotationSpeed;

    glm::mat4 baseTransform = glm::mat4(1);
    baseTransform = transform3D::Translate(position.x, position.y, position.z);
    baseTransform *= transform3D::RotateOY(rotation);
    baseTransform *= transform3D::RotateOY(glm::radians(45.0f));  // Le pun in forma de X

    // Primul paralelipiped
    glm::mat4 modelMatrix = baseTransform;
    modelMatrix *= transform3D::Scale(bodyScale.x, bodyScale.y, bodyScale.z + 0.7f);
    RenderMesh(meshes["body"], shader, modelMatrix);

    // Al doilea
    modelMatrix = baseTransform;
    modelMatrix *= transform3D::RotateOY(glm::radians(90.0f));
    modelMatrix *= transform3D::Scale(bodyScale.x, bodyScale.y, bodyScale.z + 0.7f);
    RenderMesh(meshes["body"], shader, modelMatrix);

    // Pozitiile deasupra paralelipipedelor
    float offsetDistance = 0.85f; // distanta fata de centru
    glm::vec3 cubeOffsets[4] = {
        glm::vec3((bodyScale.x / 2) + offsetDistance, 0, 0),   // Dreapa diagonala 1
        glm::vec3(-(bodyScale.x / 2) - offsetDistance, 0, 0),  // Stanga diagonala 1
        glm::vec3(0, 0, (bodyScale.x / 2) + offsetDistance),   // Dreapta diagonala 2
        glm::vec3(0, 0, -(bodyScale.x / 2) - offsetDistance)   // Stanga diagonala 2
    };

    // Pun cuburile si elicele
    for (int i = 0; i < 4; i++) {
        // Cuburile
        modelMatrix = baseTransform;
        modelMatrix *= transform3D::Translate(cubeOffsets[i].x, bodyScale.y / 2, cubeOffsets[i].z);
        RenderMesh(meshes["cube"], shader, modelMatrix);

        // Elicele peste cuburi
        modelMatrix *= transform3D::Translate(0.0f, 0.2f, 0.0f);
        modelMatrix *= transform3D::RotateOY(angle);             // Rotatia continua
        modelMatrix *= transform3D::Scale(propellerScale.x, propellerScale.y, propellerScale.z);
        RenderMesh(meshes["propeller"], shader, modelMatrix);
    }
}



void Tema2::RenderLifeBar(glm::vec3 dronePosition) {
    Shader* shader = shaders["VertexColor"];
    shader->Use();

    // Conturul barii de viata
    glm::vec3 relativeOffset = glm::vec3(-4.0f, 0.0f, -4.5f);
    glm::mat4 rotationMatrix = glm::mat4(1);
    rotationMatrix = transform3D::RotateOY(droneRotation);
    glm::vec4 rotatedOffset = rotationMatrix * glm::vec4(relativeOffset, 1.0f);
    glm::vec3 outlinePosition = dronePosition + glm::vec3(rotatedOffset);

    glm::mat4 outlineModelMatrix = glm::mat4(1);
    outlineModelMatrix = transform3D::Translate(outlinePosition.x, outlinePosition.y, outlinePosition.z);
    outlineModelMatrix *= transform3D::RotateOY(droneRotation); // Pastrez rotatia dronei stanga-dreapta

    RenderMesh(meshes["lifeBarOutline"], shader, outlineModelMatrix);

    // Bara dinamica
    if (remainingTime > 0) {
        float timeScale = remainingTime / maxTime;

        glm::mat4 lifeBarModelMatrix = glm::mat4(1);
        lifeBarModelMatrix = transform3D::Translate(outlinePosition.x, outlinePosition.y, outlinePosition.z);
        lifeBarModelMatrix *= transform3D::RotateOY(droneRotation);
        lifeBarModelMatrix *= transform3D::Translate(0.0f, -2.5f * (1.0f - timeScale), 0.0f); // Ajustez pozitia ca sa scada doar de sus
        lifeBarModelMatrix *= transform3D::Scale(1.0f, timeScale, 1.0f);  // Scade doar pe oy

        RenderMesh(meshes["lifeBar"], shader, lifeBarModelMatrix);
    }
}

void Tema2::RenderArrowToNextGate(glm::vec3 dronePosition) {
    Shader* shader = shaders["VertexColor"];
    shader->Use();

    // Iau urmatoarea poarta de parcurs
    glm::vec3 nextGatePosition = gatePositions[gateTraversalOrder[currentGateIndex]];

    // Directia catre ea
    glm::vec3 direction = glm::normalize(nextGatePosition - dronePosition);

    // Pun sageata in fata dronei
    glm::vec3 arrowOffset = glm::vec3(-sin(droneRotation) * 3.0f, 0.5f, -cos(droneRotation) * 3.0f);
    glm::vec3 arrowPosition = dronePosition + arrowOffset;

    // O aliniez cu directia catre poarta
    float angleToGate = atan2(direction.x, direction.z);

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = transform3D::Translate(arrowPosition.x, arrowPosition.y, arrowPosition.z);
    modelMatrix *= transform3D::RotateOY(angleToGate);
    modelMatrix *= transform3D::RotateOX(glm::radians(90.0f));
    modelMatrix *= transform3D::Scale(1.0f, 1.0f, 1.0f);

    RenderMesh(meshes["arrow"], shader, modelMatrix);
}


void Tema2::RenderGates() {
    Shader* shader = shaders["VertexColor"];
    shader->Use();
    glLineWidth(3.0f);

    for (int i = 0; i < gatePositions.size(); i++) {
        glm::vec3 position = gatePositions[i];
        float scale = gateScales[i];
        std::string meshName = gateMeshes[i];  // Obtin mesh-ul asociat portii

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = transform3D::Translate(position.x, position.y, position.z);
        modelMatrix *= transform3D::Scale(scale, scale, scale);
        RenderMesh(meshes[meshName], shader, modelMatrix);  // Il iau pe cel corespunzator
    }
    glLineWidth(1.0f);
}

bool Tema2::CheckDroneInGate(glm::vec3 dronePosition, glm::vec3 gatePosition) {
    float gateWidth = 2.0f;
    float gateHeight = 3.0f;

    // Ma asigur ca drona se afla la jumatate din latimea portii ox si oz iar 
    // Coordonata y a dronei trebuie sa fie cuprinsa in inaltimea portii
    return (abs(dronePosition.x - gatePosition.x) < gateWidth / 2 &&
        abs(dronePosition.y - gatePosition.y) < gateHeight &&
        abs(dronePosition.z - gatePosition.z) < gateWidth / 2);
}

void Tema2::SetGateToGreen(int gateIndex) {
    gateMeshes[gateIndex] = "greenGate";
}

void Tema2::SetGateToRed(int gateIndex) {
    gateMeshes[gateIndex] = "redGate";
}



void Tema2::FrameStart()
{
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);  // culoarea cerului
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::UpdateCamera()
{
    // Pozitia relativa fata de drona
    glm::vec3 cameraOffset = glm::vec3(0.0f, 2.0f, -4.0f); // in spate si deasupra

    // Forward drona in functie de rotatia ei
    glm::vec3 forward = glm::vec3(-sin(droneRotation), 0.0f, -cos(droneRotation));

    glm::vec3 cameraPosition = dronePosition - forward * glm::length(glm::vec2(cameraOffset.z, cameraOffset.y));

    cameraPosition.y += cameraOffset.y;

    // Camera trebuie sa primeasca inspre drona
    glm::vec3 cameraTarget = dronePosition;

    // Pentru a mentine orientarea camerei
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    auto camera = GetSceneCamera();
    camera->SetPosition(cameraPosition);

    glm::vec3 cameraDirection = glm::normalize(cameraTarget - cameraPosition);
    glm::vec3 right = glm::normalize(glm::cross(up, cameraDirection));
    glm::vec3 adjustedUp = glm::normalize(glm::cross(cameraDirection, right)); // trebuie sa ramana perpendicular pe directia camerei

    camera->SetRotation(glm::vec3(
        atan2(cameraDirection.y, glm::length(glm::vec2(cameraDirection.x, cameraDirection.z))), // ox
        -atan2(cameraDirection.x, -cameraDirection.z), // oy
        0.0f // oz
    ));

    camera->Update();
}


void Tema2::FrameEnd() {}

bool Tema2::CheckSphereCylinderCollision(glm::vec3 sphereCenter, float sphereRadius, glm::vec3 cylinderBase, glm::vec3 cylinderTop, float cylinderRadius) {
    glm::vec3 cylinderAxis = cylinderTop - cylinderBase;
    float axisLength = glm::length(cylinderAxis);
    cylinderAxis = glm::normalize(cylinderAxis);

    // vectorul de la baza la centrul sferei
    glm::vec3 baseToSphere = sphereCenter - cylinderBase;
    float projectionLength = glm::dot(baseToSphere, cylinderAxis); // aria de-a lungul intregii axe

    if (projectionLength < 0 || projectionLength > axisLength) return false; // nu am coliziune

    // cel mai apropiat punct de pe axa cilindrului la centrul sferei
    glm::vec3 closestPoint = cylinderBase + projectionLength * cylinderAxis;
    float distance = glm::length(sphereCenter - closestPoint);

    return distance < (sphereRadius + cylinderRadius);
}

bool Tema2::CheckSphereConeCollision(glm::vec3 sphereCenter, float sphereRadius, glm::vec3 coneBase, float coneHeight, float coneBaseRadius) {
    glm::vec3 coneTip = coneBase + glm::vec3(0, coneHeight, 0);
    // vectorul de la baza conului la centrul sferei
    glm::vec3 baseToSphere = sphereCenter - coneBase;
    // vectorul de la baza conului la varful lui
    glm::vec3 baseToTip = coneTip - coneBase;

    float heightSquared = glm::length2(baseToTip);
    float projection = glm::dot(baseToSphere, baseToTip) / heightSquared;

    if (projection < 0 || projection > 1) return false; // vad daca trece pe sub sau deasupra conului

    // Cel mai apropiat punct de pe axa conului de centrul sferei
    glm::vec3 closestPoint = coneBase + projection * baseToTip;
    // Raza conului scade pe masura ce ma departez de baza
    float baseRadiusAtProjection = coneBaseRadius * (1.0f - projection);
    float distance = glm::length(sphereCenter - closestPoint);

    return distance < (sphereRadius + baseRadiusAtProjection);
}

bool Tema2::CheckSphereAABB(glm::vec3 sphereCenter, float sphereRadius, glm::vec3 boxMin, glm::vec3 boxMax) {
    // Cel mai apropiat punct de pe axa AABB la centrul sferei
    glm::vec3 closestPoint = glm::clamp(sphereCenter, boxMin, boxMax);
    float distance = glm::length(closestPoint - sphereCenter);
    return distance < sphereRadius;
}

void Tema2::OnInputUpdate(float deltaTime, int mods) {
    if (isGameOver) {
        return;  // Nu ma mai misca daca s-a terminat jocul
    }

    glm::vec3 forward = glm::vec3(sin(droneRotation), 0, cos(droneRotation));
    glm::vec3 right = glm::vec3(cos(droneRotation), 0, -sin(droneRotation));
    glm::vec3 up = glm::vec3(0, 1, 0);

    glm::vec3 newPosition = dronePosition; // Incep de la pozitia curenta
    float droneRadius = 1.0f; // Sfera dronei

    if (window->KeyHold(GLFW_KEY_W)) {
        newPosition -= forward * moveSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        newPosition += forward * moveSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        newPosition -= right * moveSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        newPosition += right * moveSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_Q)) {
        newPosition += up * moveSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_E)) {
        newPosition -= up * moveSpeed * deltaTime;
    }

    // Am gasit coliziune sau nu
    bool canMove = true;

    // Coliziunea cu pomii
    for (size_t i = 0; i < treePositions.size(); ++i) {
        float scale = treeScales[i];

        // Trunchiul = cilindru
        glm::vec3 trunkBase = treePositions[i];
        glm::vec3 trunkTop = trunkBase + glm::vec3(0, 1.2f * scale, 0);
        float trunkRadius = 0.1f * scale;

        if (CheckSphereCylinderCollision(newPosition, droneRadius, trunkBase, trunkTop, trunkRadius)) {
            canMove = false;
            break;
        }

        // FRunzele = conuri
        glm::vec3 coneBase = trunkTop;
        float coneHeight = 1.0f * scale;
        float coneBaseRadius = 0.5f * scale;

        if (CheckSphereConeCollision(newPosition, droneRadius, coneBase, coneHeight, coneBaseRadius)) {
            canMove = false;
            break;
        }
    }

    // Coliziunea cu casele
    for (size_t i = 0; i < housePositions.size(); ++i) {
        float scale = houseScales[i];

        // Baza casei
        glm::vec3 houseMin = housePositions[i] - glm::vec3(scale + 4.0f, 0.0f, scale + 3.0f);
        glm::vec3 houseMax = housePositions[i] + glm::vec3(scale + 4.0f, 2.0f * scale, scale + 3.0f);

        if (CheckSphereAABB(newPosition, droneRadius, houseMin, houseMax)) {
            canMove = false;
            break;
        }

        // Acoperis = con
        glm::vec3 roofBase = housePositions[i] + glm::vec3(0, 2.0f * scale, 0);
        float roofHeight = scale + 4.0f;
        float roofBaseRadius = scale + 4.0f;

        if (CheckSphereConeCollision(newPosition, droneRadius, roofBase, roofHeight, roofBaseRadius)) {
            canMove = false;
            break;
        }
    }

    // Coliziunea cu terenul
    if (newPosition.y < 2.0f) {
        canMove = false;
    }

    // Actualizez pozitia daca nu am coliziune
    if (canMove) {
        dronePosition = newPosition;
    }

    // Rotatia dronei
    if (window->KeyHold(GLFW_KEY_LEFT)) {
        droneRotation += rotationSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        droneRotation -= rotationSpeed * deltaTime;
    }
}

void Tema2::RenderGameOverText()
{
    if (textRenderer) {
        glm::ivec2 resolution = window->GetResolution();
        float x = resolution.x / 2.0f - 200.0f; // Pun textul in centru
        float y = resolution.y / 2.0f;

        if (isSuccess) {
            textRenderer->RenderText("SUCCESS", x, y, 1.5f, glm::vec3(0.0f, 1.0f, 0.0f));
        }
        else if (isGameOver) {
            textRenderer->RenderText("GAME OVER", x, y, 1.5f, glm::vec3(1.0f, 0.0f, 0.0f));
        }
    }
}



void Tema2::OnKeyPress(int key, int mods) {}

void Tema2::OnKeyRelease(int key, int mods) {}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void Tema2::OnWindowResize(int width, int height) {}
