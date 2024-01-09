#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:

    GLuint ID;

    Shader(const char* vertex_path, const char* fragment_path)
    {
        //retrieve shader source code from file path
        std::string vertex_code;
        std::string fragment_code;
        std::ifstream vertex_shader_file;
        std::ifstream fragment_shader_file;

        //ensure ifstream objects can throw exceptions
        vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            //open files
            vertex_shader_file.open(vertex_path);
            fragment_shader_file.open(fragment_path);
            std::stringstream vertex_shader_stream, fragment_shader_stream;

            //read file's buffer contents into streams
            vertex_shader_stream << vertex_shader_file.rdbuf();
            fragment_shader_stream << fragment_shader_file.rdbuf();

            //close file handlers
            vertex_shader_file.close();
            fragment_shader_file.close();

            //convert stream into string
            vertex_code = vertex_shader_stream.str();
            fragment_code = fragment_shader_stream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }

        const char* vertex_shader_code = vertex_code.c_str();
        const char* fragment_shader_code = fragment_code.c_str();

        //compile shaders
        GLuint vertex_shader_id, fragment_shader_id;

        //vertex shader
        vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader_id, 1, &vertex_shader_code, nullptr);
        glCompileShader(vertex_shader_id);
        checkCompileErrors(vertex_shader_id, "VERTEX");

        //fragment Shader
        fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader_id, 1, &fragment_shader_code, nullptr);
        glCompileShader(fragment_shader_id);
        checkCompileErrors(fragment_shader_id, "FRAGMENT");

        //shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex_shader_id);
        glAttachShader(ID, fragment_shader_id);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        //delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use()
    {
        glUseProgram(ID);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

private:
    
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        GLint success = GL_TRUE;
        GLchar info_log[1024];

        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (success == GL_FALSE)
            {
                glGetShaderInfoLog(shader, sizeof(info_log), nullptr, info_log);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << info_log << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (success == GL_FALSE)
            {
                glGetProgramInfoLog(shader, sizeof(info_log), nullptr, info_log);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << info_log << std::endl;
            }
        }
    }
};
