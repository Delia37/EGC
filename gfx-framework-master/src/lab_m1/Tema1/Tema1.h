#pragma once

#include "components/simple_scene.h"
#include <vector>

namespace m1
{
    struct Projectile {
        glm::vec2 position;
        bool active = false; // pt a verifica daca proiectilul se misca
        std::vector<glm::vec2> trajectory; // punctele de pe traiectorie
        size_t currentPoint = 0;
    };
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

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
        void RenderTank(float tankX, float tankAngle, const std::string& tankPrefix, float yOffset);
        std::vector<glm::vec2> GenerateTrajectoryPoints(float angle, float magnitude, float initialPosition, float g);
        void UpdateProjectile(Projectile& projectile, float deltaTime);
        void DeformTerrain(float explosionX, float radius);
        void ApplyLandslideAnimation(float deltaTime);
        std::vector<glm::vec2> GetVisibleTrajectoryPoints(const std::vector<glm::vec2>& trajectoryPoints,
            float barrelTipX, const std::vector<int>& heightMap);

    protected:
        glm::mat3 modelMatrix;
        float pointSpacing;
        float tankX1;
        float tankX2;
        float tankSpeed;
        float tankAngle1;
        float tankAngle2;

        float barrelAngle1;
        float barrelAngle2;

        glm::vec2 barrelTipPosition1;
        glm::vec2 barrelTipPosition2;

        float tank1Health, tank2Health;

        std::vector<int> heightMap;

        std::vector<Projectile> projectiles1;
        std::vector<Projectile> projectiles2;


    };
}
