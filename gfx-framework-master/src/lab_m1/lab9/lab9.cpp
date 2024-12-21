#include "lab_m1/lab9/lab9.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab9::Lab9()
{
}


Lab9::~Lab9()
{
}


void Lab9::Init()
{
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab9", "textures");

    // Load textures
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "grass_bilboard.png").c_str(), GL_REPEAT);
        mapTextures["grass"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "crate.jpg").c_str(), GL_REPEAT);
        mapTextures["crate"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "earth.png").c_str(), GL_REPEAT);
        mapTextures["earth"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "vegetation", "bamboo", "bamboo.png").c_str(), GL_REPEAT);
        mapTextures["bamboo"] = texture;
    }

    {
        mapTextures["random"] = CreateRandomTexture(25, 25);
    }

    // Load meshes
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("bamboo");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "vegetation", "bamboo"), "bamboo.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a simple quad
    {
        vector<glm::vec3> vertices
        {
            glm::vec3(0.5f,   0.5f, 0.0f),    // top right
            glm::vec3(0.5f,  -0.5f, 0.0f),    // bottom right
            glm::vec3(-0.5f, -0.5f, 0.0f),    // bottom left
            glm::vec3(-0.5f,  0.5f, 0.0f),    // top left
        };

        vector<glm::vec3> normals
        {
            glm::vec3(0, 1, 1),
            glm::vec3(1, 0, 1),
            glm::vec3(1, 0, 0),
            glm::vec3(0, 1, 0)
        };

        vector<glm::vec2> textureCoords
        {
            // TODO(student): Complete texture coordinates for the square
            //glm::vec2(0.0f, 0.0f)
            glm::vec2(0.0f, 0.0f), // Bottom left
            glm::vec2(0.0f, 1.0f),  // Top left
            glm::vec2(1.0f, 1.0f), // Top right
            glm::vec2(1.0f, 0.0f), // Bottom right


        };

        vector<unsigned int> indices =
        {
            0, 1, 3,
            1, 2, 3
        };

        Mesh* mesh = new Mesh("square");
        mesh->InitFromData(vertices, normals, textureCoords, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab9", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab9", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


void Lab9::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab9::Update(float deltaTimeSeconds)
{
    // TODO(student): Choose an object and add a second texture to it.
    // For example, for the sphere, you can have the "earth" texture
    // and the "random" texture, and you will use the `mix` function
    // in the fragment shader to mix these two textures.
    //
    // However, you may have the unpleasant surprise that the "random"
    // texture now appears onto all objects in the scene, even though
    // you are only passing the second texture for a single object!
    // Why does this happen? How can you solve it?
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(1, 1, -3));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
        //RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, mapTextures["earth"]);
        RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, mapTextures["earth"], nullptr, false, true);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75f));
        //RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["crate"]);
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["crate"], nullptr, false, false);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75f));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(75.0f), glm::vec3(1, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["random"], nullptr, false, false);
    }

    //{
    //    glm::mat4 modelMatrix = glm::mat4(1);
    //    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
    //    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
    //    //RenderSimpleMesh(meshes["square"], shaders["LabShader"], modelMatrix, mapTextures["grass"]);
    //    RenderSimpleMesh(meshes["square"], shaders["LabShader"], modelMatrix, mapTextures["grass"], nullptr, false, false);
    //}
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        glm::vec3 grassPosition = glm::vec3(0.0f, 0.5f, 0.0f);

        // Get the camera position
        glm::vec3 cameraPosition = GetSceneCamera()->m_transform->GetWorldPosition();

        // Compute the direction vector (ignoring Y axis)
        glm::vec3 direction = glm::normalize(glm::vec3(cameraPosition.x, 0.0f, cameraPosition.z) - glm::vec3(grassPosition.x, 0.0f, grassPosition.z));

        // Compute the angle to rotate the quad
        float angle = atan2(direction.x, direction.z);

        // Apply transformations: Translate -> Rotate -> Scale
        modelMatrix = glm::translate(modelMatrix, grassPosition);
        modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));

        // Render the grass quad
        RenderSimpleMesh(meshes["square"], shaders["LabShader"], modelMatrix, mapTextures["grass"], nullptr, false, false);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, -0.5f, -3));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderSimpleMesh(meshes["bamboo"], shaders["LabShader"], modelMatrix, mapTextures["bamboo"], nullptr, false, false);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.7f, 0.0f, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.7f));

        float mixFactorValue = 0.5f;

        // Activate shader program
        glUseProgram(shaders["LabShader"]->GetProgramID());

        // Set the mix factor uniform
        GLint loc_mix_factor = glGetUniformLocation(shaders["LabShader"]->GetProgramID(), "mix_factor");
        glUniform1f(loc_mix_factor, mixFactorValue);

        // Render the quad
        RenderSimpleMesh(meshes["square"], shaders["LabShader"], modelMatrix, mapTextures["random"], mapTextures["earth"],
            true, false);
    }
}


void Lab9::FrameEnd()
{
    DrawCoordinateSystem();
}


void Lab9::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, Texture2D* texture2,
    bool useMix, bool useTimeAnimation)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Use the shader program
    glUseProgram(shader->program);

    // Bind model, view, and projection matrices
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "View"), 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Projection"), 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));

    // Bind the first texture (texture1)
    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0); // Activate texture unit 0
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID()); // Bind the texture to unit 0
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0); // Map sampler2D to unit 0
    }

    // Bind the second texture (texture2), if provided
    if (texture2)
    {
        glActiveTexture(GL_TEXTURE1); // Activate texture unit 1
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID()); // Bind the texture to unit 1
        glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1); // Map sampler2D to unit 1
    }

    // Send useMix to the shader
    glUniform1i(glGetUniformLocation(shader->program, "useMix"), useMix);

    // Send useTimeAnimation to the shader
    glUniform1i(glGetUniformLocation(shader->program, "useTimeAnimation"), useTimeAnimation);

    // If time animation is enabled, send the elapsed time
    if (useTimeAnimation) {
        float elapsedTime = Engine::GetElapsedTime();
        glUniform1f(glGetUniformLocation(shader->program, "elapsedTime"), elapsedTime);
    }

    // Send mix factor to the shader if useMix is enabled
    if (useMix) {
        float mixFactorValue = 0.5f; // Static or dynamic value
        glUniform1f(glGetUniformLocation(shader->program, "mix_factor"), mixFactorValue);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


Texture2D* Lab9::CreateRandomTexture(unsigned int width, unsigned int height)
{
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    // TODO(student): Generate random texture data
    for (unsigned int i = 0; i < size; ++i) {
        data[i] = rand() % 256; // Random values between 0 and 255
    }

    // TODO(student): Generate and bind the new texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    // TODO(student): Set the texture parameters (MIN_FILTER, MAG_FILTER and WRAPPING MODE) using glTexParameteri
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    // Use glTexImage2D to set the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // TODO(student): Generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);

    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    delete[] data;
    return texture;
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab9::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
    }
}


void Lab9::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Lab9::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab9::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab9::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab9::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab9::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab9::OnWindowResize(int width, int height)
{
}
