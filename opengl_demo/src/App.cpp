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

/* Window Width-Height */
constexpr auto width = 1680;
constexpr auto height = 1050;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        exit(0);
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
        float positions[] = {
           -5.0f, -5.0f, -15.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f,
            5.0f, -5.0f, -15.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,
            5.0f,  5.0f, -15.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,
           -5.0f,  5.0f, -15.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f,
                                 
           -5.0f, -5.0f, -5.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f,
            5.0f, -5.0f, -5.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,
            5.0f,  5.0f, -5.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,
           -5.0f,  5.0f, -5.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f
        };

        unsigned int indices[] = {
             0, 1, 2,
             2, 3, 0,
        };

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);

        //VertexArray va;
        VertexBuffer vb(positions,  sizeof(positions));

        VertexBufferLayout layout;
        //layout.Push<float>(4);
        //layout.Push<float>(4);

        unsigned int va;
        glGenVertexArrays(1, &va);
        glBindVertexArray(va);


        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(4 * sizeof(float)));


        //va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        Shader shader("res/shaders/vs.shader", "res/shaders/fs.shader");
        shader.Bind();

        //Texture texture("res/textures/covid.png");
        //texture.Bind();
        //shader.SetUniform1i("u_Texture", 0);

        //va.UnBind();
        glBindVertexArray(0);
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        /*ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();*/

        glm::vec3 look(0.0f, 0.0f, 0.0f);
        glm::vec3 position(0.0f, 0.0f, 10.0f);

        PCamera camera(45.0f, (float)width / (float)height, 0.1f, 100.0f);
        camera.SetPosition(glm::vec3(0.0f, 0.0f, 10.0f));
        camera.SetLookAt(glm::vec3(0.0f, 0.0f, 0.0f));

        Movement camera_movement(window);
      
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            //ImGui_ImplGlfwGL3_NewFrame();

            shader.Bind();

            glfwSetKeyCallback(window, key_callback);

            camera_movement.OnUpdate();

            camera.SetPosition(camera_movement.GetPosition());
            camera.SetLookAt(camera_movement.GetDirection() + camera_movement.GetPosition());
            shader.SetUniformMat4f("u_MVP", camera.GetViewProjectionMatrix());

            //renderer.Draw(va, ib, shader);
            shader.Bind();
            glBindVertexArray(va);
            ib.Bind();
            glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);


            ///* IamGUI */
            //{
            //    ImGui::Text("Hello, to your mama");                           
            //    ImGui::SliderFloat3("TranslationA", &position.x, -90.0f, 90.0f); 
            //    ImGui::SliderFloat3("look", &look.x, -10.0f, +10.0f);
            //}

            //ImGui::Render();
            //ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

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