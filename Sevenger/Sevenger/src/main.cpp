#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb_image.h>
#include<iostream>

//settings
const GLuint SCR_WIDTH = 1920;
const GLuint SCR_HEIGHT = 1080;

//vertex shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//fragment shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n\0";

//input
void processInput(GLFWwindow* window)
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Solar System", nullptr, nullptr);
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
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertex_shader_id);

    //check for vertex shader compile errors
    GLint success = GL_TRUE;
    GLchar info_log[512];
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(vertex_shader_id, sizeof(info_log), nullptr, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    //build and compile fragment shader
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragment_shader_id);

    // check for shader compile errors
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(fragment_shader_id, sizeof(info_log), nullptr, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    // link shaders
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader_id);
    glAttachShader(shader_program, fragment_shader_id);
    glLinkProgram(shader_program);
    // check for linking errors
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, sizeof(info_log), nullptr, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
       -0.5f, -0.5f, 0.0f,  // bottom left
       -0.5f,  0.5f, 0.0f   // top left    
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    //VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    //uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //main loop
    while (!glfwWindowShouldClose(window))
    {
        //input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //draw first triangle
        glUseProgram(shader_program);
        glBindVertexArray(VAO_id);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //glfw swap buffers
        glfwSwapBuffers(window);

        //glfw poll events
        glfwPollEvents();
    }

    //optional: de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO_id);
    glDeleteBuffers(1, &VBO_id);
    glDeleteBuffers(1, &EBO_id);
    glDeleteProgram(shader_program);

    //glfw terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

