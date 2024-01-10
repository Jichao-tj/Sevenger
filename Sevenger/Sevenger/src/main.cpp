#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

GLint SCREEN_WIDTH = 800;
GLint SCREEN_HEIGHT = 600;
bool detection_on = false;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        detection_on = !detection_on;
    }
}

GLuint load_texture(const char* path) 
{
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    if (data) {
        GLenum format;
        if (channels == 1)
            format = GL_RED;
        else if (channels == 3)
            format = GL_RGB;
        else if (channels == 4)
            format = GL_RGBA;
        else {
            stbi_image_free(data);
            return 0; 
        }

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);  // Unbind the texture
    }
    else {
        std::cout << "Failed to load texture: " << path << std::endl;
        return 0;
    }

    return texture_id;
}

int main()
{
    //glfw initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Edge Detection", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //glad load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //build and compile shader
    Shader edge_detection("assets/shaders/edge_detection.vs", "assets/shaders/edge_detection.fs");
    Shader texture_shader("assets/shaders/texture.vs"       , "assets/shaders/texture.fs");

    //set up vertex data and buffer, configure vertex attributes
    float vertices[] = {
          // positions         // colors           // texture coords
          0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
          0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
         -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
         -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left   
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

    //describe vertex data layout
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //load and create texture 
    GLuint texture_ID = load_texture("assets/textures/world_map.png");
    //GLuint texture_ID = load_texture("assets/textures/Tex_A1x.png");
    //GLuint texture_ID = load_texture("assets/textures/awesomeface.png");
    //GLuint texture_ID = load_texture("assets/textures/Tex_4.png");
 
    //main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        //process input
        glfwSetKeyCallback(window, key_callback);
        
        //clear
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //render
        glBindTexture(GL_TEXTURE_2D, texture_ID);
        (detection_on) ? edge_detection.use() : texture_shader.use();
        glBindVertexArray(VAO_id);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //glfw swap buffers
        glfwSwapBuffers(window);

        //glfw poll events
        glfwPollEvents();
    }

    //de-allocate
    glDeleteVertexArrays(1, &VAO_id);
    glDeleteBuffers(1, &VBO_id);
    glDeleteBuffers(1, &EBO_id);

    glfwTerminate();
    return 0;
}
