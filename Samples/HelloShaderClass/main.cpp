// Local Headers
#include "glitter.hpp"

// System Headers
// Be sure to include GLAD before GLFW. The include file for GLAD includes the required OpenGL headers 
// behind the scenes (like GL/gl.h) so be sure to include GLAD before other header files that require 
// OpenGL (like GLFW).
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyEvnt_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Define Some Constants
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

#define _Change_Green_color_ 1

#if _Change_Green_color_
float vertices[] = {
    0.5f,  0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
#else
float vertices[] = {
    // positions          // colors
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   // bottom right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   // bottom left
     -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   // top 
     -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f   // top left 
};  
#endif
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
};

int frameState = 0;

int main(int argc, char * argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        //fallback and try again
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        mWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
        if (mWindow == nullptr) {
            cerr << "Failed to Create OpenGL Context\n";
            return EXIT_FAILURE;
        }
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);

    glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
    glfwSetKeyCallback(mWindow, keyEvnt_callback);

    gladLoadGL();
    cout << "OpenGL " << glGetString(GL_VERSION) << "\n";


    // build and compile our shader program
    // ------------------------------------
    #if _Change_Green_color_
        Shader ourShader("shaders/HSC_shader1.vs", "shaders/HSC_shader1.fs");
    #else
        Shader ourShader("shaders/HSC_shader.vs", "shaders/HSC_shader.fs");
    #endif


    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

#if _Change_Green_color_
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
#else
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
#endif

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {

        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        ourShader.use();

        #if _Change_Green_color_
        // update shader uniform
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        ourShader.setVec4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
        #endif
        
        if(frameState!=0){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//only draw line
        }
        else{
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        
    }   

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return EXIT_SUCCESS;
}


// Is called whenever a key is pressed/released via GLFW
void keyEvnt_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key == GLFW_KEY_1 && action == GLFW_PRESS){
        if(frameState!=0){
            frameState = 0;
        }
        else{
            frameState = 1;
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    cout << "Set window wxh " << width << "x" << height << "\n";
    glViewport(0, 0, width, height);
}
