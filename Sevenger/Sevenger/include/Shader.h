#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:

    GLuint shader_program_id;

    Shader(const char* vertex_path, const char* fragment_path)
    {
        //load shader source code
        // ----------------------
        std::string vertex_code;
        std::string fragment_code;
        std::ifstream vertex_shader_file;
        std::ifstream fragment_shader_file;
        //throw exceptions if file reading failed
        vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            //open file
            vertex_shader_file.open(vertex_path);
            fragment_shader_file.open(fragment_path);
            std::stringstream vertex_shader_stream, fragment_shader_stream;
            //read file into stream
            vertex_shader_stream << vertex_shader_file.rdbuf();
            fragment_shader_stream << fragment_shader_file.rdbuf();
            //close file
            vertex_shader_file.close();
            fragment_shader_file.close();
            //convert stream to string
            vertex_code = vertex_shader_stream.str();
            fragment_code = fragment_shader_stream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR: SHADER FILE READING FAILED: " << e.what() << std::endl;
        }
        const char* vertex_shader_code = vertex_code.c_str();
        const char* fragment_shader_code = fragment_code.c_str();
        // ------------------------------------------------------
        
        //compile shader
        // -------------
        GLuint vertex_shader_id, fragment_shader_id;

        //vertex shader
        vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader_id, 1, &vertex_shader_code, nullptr);
        glCompileShader(vertex_shader_id);
        check_compile_errors(vertex_shader_id, "VERTEX SHADER");

        //fragment shader
        fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader_id, 1, &fragment_shader_code, nullptr);
        glCompileShader(fragment_shader_id);
        check_compile_errors(fragment_shader_id, "FRAGMENT SHADER");

        //shader program
        shader_program_id = glCreateProgram();
        glAttachShader(shader_program_id, vertex_shader_id);
        glAttachShader(shader_program_id, fragment_shader_id);
        glLinkProgram(shader_program_id);
        check_compile_errors(shader_program_id, "SHADER PROGRAM");
        // ------------------------------------------------
        
        //delete shaders after they are linked
        glDetachShader(shader_program_id, vertex_shader_id);
        glDetachShader(shader_program_id, fragment_shader_id);
        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
    }
    
    void use()
    {
        glUseProgram(shader_program_id);
    }

    //uniform functions
    void set_bool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(shader_program_id, name.c_str()), (int)value);
    }

    void set_int(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(shader_program_id, name.c_str()), value);
    }

    void set_float(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(shader_program_id, name.c_str()), value);
    }

private:
    
    void check_compile_errors(unsigned int shader, std::string type)
    {
        GLint compile_status = GL_TRUE;
        GLchar error_message[1024];

        if (type != "SHADER PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
            if (compile_status == GL_FALSE)
            {
                glGetShaderInfoLog(shader, sizeof(error_message), nullptr, error_message);
                std::cout << "ERROR: SHADER COMPILATION ERROR of type: " << type << "\n" << error_message << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &compile_status);
            if (compile_status == GL_FALSE)
            {
                glGetProgramInfoLog(shader, sizeof(error_message), nullptr, error_message);
                std::cout << "ERROR: PROGRAM LINKING ERROR of type: " << type << "\n" << error_message << std::endl;
            }
        }
    }
};
