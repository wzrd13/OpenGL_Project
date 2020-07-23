/* GLEW GLFW */
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* OpenGL API */
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Texture.h"
#include "PCamera.h"
#include "Movement.h"
#include "Mesh.h"

/* GLM */
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm\gtx\string_cast.hpp>
//#include <glm/gtx/transform.hpp>

/* ImGUI */
#include "imgui\imgui_impl_glfw_gl3.h"
#include <imgui\imgui.h>

#include <iostream>
#include <string>
#include <vector>
#include <array>

/* Window Width-Height */
constexpr auto width = 1680;
constexpr auto height = 1050;

Movement* camera_movement;

void imgui()
{
    // Place outside while loop
    /*ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();*/
    ImGui_ImplGlfwGL3_NewFrame();

    /* IamGUI */
    {
        ImGui::Text("Hello, to your mama");
        //ImGui::SliderFloat3("TranslationA", &position.x, -90.0f, 90.0f);
        //ImGui::SliderFloat3("look", &look.x, -10.0f, +10.0f);
    }

    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        camera_movement->SetEscapeFlag();
}

static std::vector<Vertex> CreateQuad(glm::vec3 position, unsigned int size, glm::vec4 color, float textureid)
{
    Vertex v0;
    v0.Position = { position.x, position.y, position.z, 1.0f };
    v0.Color = color;
    v0.Normal = { 0.0f, 0.0f, 0.0f };
    v0.TexCoords = { 0.0f, 0.0f };

    Vertex v1;
    v1.Position = { position.x + size, position.y, position.z, 1.0f };
    v1.Color = color;
    v1.Normal = { 0.0f, 0.0f, 0.0f };
    v1.TexCoords = { 1.0f, 0.0f };

    Vertex v2;
    v2.Position = { position.x + size,  position.y + size, position.z, 1.0f };
    v2.Color = color;
    v2.Normal = { 0.0f, 0.0f, 0.0f };
    v2.TexCoords = { 1.0f, 1.0f };

    Vertex v3;
    v3.Position = { position.x,  position.y + size, position.z, 1.0f };
    v3.Color = color;
    v3.Normal = { 0.0f, 0.0f, 0.0f };
    v3.TexCoords = { 0.0f, 1.0f };
    
    return { v0, v1, v2, v3 };
}

static std::vector<Vertex> Cube(glm::vec3 position, unsigned int size, glm::vec4 color, float textureid)
{
    auto q1 = CreateQuad(glm::vec3(position.x, position.y, position.z), size, color, textureid);
    auto q2 = CreateQuad(glm::vec3(position.x, position.y, position.z + size), size, color, textureid);

    q1.insert(
        q1.end(),
        std::make_move_iterator(q2.begin()),
        std::make_move_iterator(q2.end())
    );

    return q1;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the GLFW library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    glfwSwapInterval(1);

    unsigned int err;


    /* Initialize the GLEW library */
    if ((err = glewInit())) 
    {
        std::cout << "failed to initialize GLEW.\n" << glewGetErrorString(err) << std::endl;
        return -1;
    }
    else 
    {
        std::cout << "GLEW OK\n" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        // Index Vector
        std::vector<unsigned int> cube = {
            // front
            0, 1, 2,
            2, 3, 0,
            // right
            1, 5, 6,
            6, 2, 1,
            // back
            7, 6, 5,
            5, 4, 7,
            // left
            4, 0, 3,
            3, 7, 4,
            // bottom
            4, 5, 1,
            1, 0, 4,
            // top
            3, 2, 6,
            6, 7, 3
        };

        // Create Cubes Cordinates
        glm::vec3 position1(5.0f, 5.0f, 5.0f);
        glm::vec3 position2(-5.0f, 15.0f, 0.0f);
        auto q1 = Cube(position1, 4, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.0f);
        auto q2 = Cube(position2, 1, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f);

        // Create Cubes
        Mesh mesh1(q1, cube);
        Mesh mesh2(q2, cube);


        // Gl Blend for textures
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);

        // Create Shaders
        Shader shader("res/shaders/vs.shader", "res/shaders/fs.shader");
        Shader light_shader("res/shaders/vs.shader", "res/shaders/fs_light.shader");
        std::cout << light_shader.GetID() << std::endl;
        std::cout << shader.GetID() << std::endl;

        PCamera camera(45.0f, (float)width / (float)height, 0.1f, 100.0f);
        camera.SetPosition(glm::vec3(0.0f, 0.0f, 50.0f));
        camera.SetLookAt(glm::vec3(0.0f, 0.0f, 0.0f));

        camera_movement = new Movement(window);

        /*ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();*/
      
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            
            glfwSetKeyCallback(window, key_callback);

            /* IamGUI */
            /*{
                ImGui_ImplGlfwGL3_NewFrame();
                ImGui::Text("Hello, to your mama");
                ImGui::SliderFloat3("TranslationA", &position2.x, -90.0f, 90.0f);
                ImGui::Render();
                ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
            }*/


            mesh2.Draw(light_shader);

            mesh1.Draw(shader);

            camera_movement->OnUpdate();

            camera.SetPosition(camera_movement->GetPosition());
            camera.SetLookAt(camera_movement->GetDirection() + camera_movement->GetPosition());
            shader.SetUniformMat4f("u_MVP", camera.GetViewProjectionMatrix());
            //light_shader.SetUniformMat4f("u_MVP", camera.GetViewProjectionMatrix());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
        
    }
    //ImGui_ImplGlfwGL3_Shutdown();
    //ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}