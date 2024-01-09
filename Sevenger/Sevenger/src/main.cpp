#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb_image.h>

#include "Shader.h"

//settings
const GLuint SCREEN_WIDTH = 1920;
const GLuint SCREEN_HEIGHT = 1080;

//input
void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//function executes when window size change
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //make sure the viewport matches the new window dimensions
    glViewport(0, 0, width, height);
}

int main()
{
    //glfw initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Solar System", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //glad load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //build and compile vertex shader
    Shader shader("assets/shader.vs", "assets/shader.fs");

    //set up vertex data and buffer, configure vertex attributes
    float vertices[] = {
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top   
    };

    unsigned int indices[] = {
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    //create and bind Vertex Array Object (VAO)
    GLuint VAO_id = 0;
    glGenVertexArrays(1, &VAO_id);
    glBindVertexArray(VAO_id);

    //create and bind Vertex Buffer Object (VBO)
    GLuint VBO_id = 0;
    glGenBuffers(1, &VBO_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //create and bind Element Buffer Object (EBO)
    GLuint EBO_id = 0;
    glGenBuffers(1, &EBO_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //describe the vertex data layout
    //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //unbind VAO
    glBindVertexArray(0);

    //uncomment this call to draw in wireframe polygons
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //main loop
    while (!glfwWindowShouldClose(window))
    {
        //input
        // -----
        process_input(window);

        // render
        // ------
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //draw first triangle
        shader.use();

        glBindVertexArray(VAO_id);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //glfw swap buffers
        glfwSwapBuffers(window);

        //glfw poll events
        glfwPollEvents();
    }

    //optional: de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO_id);
    glDeleteBuffers(1, &VBO_id);
    glDeleteBuffers(1, &EBO_id);

    //glfw terminate, clearing all previously allocated GLFW resources
    glfwTerminate();
    return 0;
}

