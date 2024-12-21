#pragma once
#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderTrees();
        void RenderHouses();
        void RenderTerrain(glm::vec3 position, glm::vec3 scale);
        void RenderDrone(glm::vec3 position, float rotation, glm::vec3 bodyScale, glm::vec3 propellerScale, float rotationSpeed);
        void UpdateCamera();
        void RenderGates();
        void RenderArrowToNextGate(glm::vec3 dronePosition);
        bool CheckDroneInGate(glm::vec3 dronePosition, glm::vec3 gatePosition);
        void RenderLifeBar(glm::vec3 dronePosition);
        void ChangeGateColor(int gateIndex, glm::vec3 newColor);
        void RenderGameOverText();
        void RenderText(const std::string& text, float x, float y, float scale, glm::vec3 color);
        void SetGateToGreen(int gateIndex);
        void SetGateToRed(int gateIndex);
        bool CheckSphereConeCollision(glm::vec3 sphereCenter, float sphereRadius, glm::vec3 coneBase, float coneHeight, float coneBaseRadius);
        bool CheckSphereCylinderCollision(glm::vec3 sphereCenter, float sphereRadius, glm::vec3 cylinderBase, glm::vec3 cylinderTop, float cylinderRadius);
        bool CheckSphereAABB(glm::vec3 sphereCenter, float sphereRadius, glm::vec3 boxMin, glm::vec3 boxMax);
        void InitializeGates(float terrainStartX, float terrainEndX, float terrainStartZ, float terrainEndZ);
        glm::vec3 GetRandomPosition(float terrainStartX, float terrainEndX, float terrainStartZ, float terrainEndZ, float height);
    protected:
        glm::vec3 dronePosition;
        float droneRotation;
        float moveSpeed;
        float rotationSpeed;
        std::vector<glm::vec3> treePositions;
        std::vector<float> treeScales;
        std::vector<glm::vec3> housePositions;
        std::vector<float> houseScales;
        std::vector<glm::vec3> gatePositions;
        std::vector<float> gateScales;
        int currentGateIndex;
        std::vector<int> gateTraversalOrder;
        std::vector<glm::vec3> gateColors;
        bool isGameOver;
        bool isSuccess;
        gfxc::TextRenderer* textRenderer;

    };
}   // namespace m1

