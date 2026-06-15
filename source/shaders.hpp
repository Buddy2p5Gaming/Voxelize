#pragma once

/* Include */
// Voxelize
#include "basic.hpp"

/* Shaders */
namespace Voxelize::Graphics {
    // constants
    const Voxelize::U64 OpenGLErrorLogMaxLength = 2048;

    // one shader
    class Shader {
    public:
        // shader data
        GLuint shader_ID;
        std::string code;

        // error data
        std::string opengl_log;
        GLint opengl_log_length;
        GLenum opengl_error_code;

        // constructors
        Shader() {
            shader_ID = 0;
            code = "";
            opengl_log = "";
            opengl_log_length = 0;
            opengl_error_code = GL_NO_ERROR;
        }
        // open and compile shader
        void OpenShader(std::string _code, Voxelize::Error* _error, GLenum _shader_type) {
            // create new shader
            shader_ID = glCreateShader(_shader_type);

            // setup temp
            GLchar* temp = (GLchar*)_code.c_str();

            // send source code to gpu
            glShaderSource(shader_ID, 1, (const GLchar* const*)&temp, NULL);

            // compiler the shader
            glCompileShader(shader_ID);

            // check for opengl errors
            glGetShaderiv(shader_ID, GL_INFO_LOG_LENGTH, &opengl_log_length);
            if (opengl_log_length > 0) {
                // create place for opengl to dump log
                Voxelize::Character log[Voxelize::Graphics::OpenGLErrorLogMaxLength];

                // get opengl error data
                glGetShaderInfoLog(shader_ID, Voxelize::Graphics::OpenGLErrorLogMaxLength, NULL, (GLchar*)&log);
                opengl_error_code = glGetError();

                // convert log error to string
                opengl_log = std::string((char*)&log, opengl_log_length);

                // generate Voxelize error
                if (_shader_type == GL_VERTEX_SHADER) {
                    *_error = Voxelize::Error(true, "\"error\": {\n\t\"reason\": \"OpenGL vertex shader did not compile.\",\n\t\"OpenGL Error Code\": " + std::to_string((int)opengl_error_code) + ",\n\t\"OpenGL Log\": \"" + opengl_log + "\"\n}");
                } else if (_shader_type == GL_FRAGMENT_SHADER) {
                    *_error = Voxelize::Error(true, "\"error\": {\n\t\"reason\": \"OpenGL fragment shader did not compile.\",\n\t\"OpenGL Error Code\": " + std::to_string((int)opengl_error_code) + ",\n\t\"OpenGL Log\": \"" + opengl_log + "\"\n}");
                }
            }

            return;
        }

        // close shader
        void CloseShader() {
            glDeleteShader(shader_ID);
        }
    };

    // one shader program
    class Shaders {
    public:
        // shader program data
        GLuint program_ID;
        Voxelize::Graphics::Shader vertex_shader;
        Voxelize::Graphics::Shader fragment_shader;

        // error data
        std::string opengl_log;
        GLint opengl_log_length;
        GLenum opengl_error_code;

        // constructors
        Shaders() {
            program_ID = 0;
            vertex_shader = Voxelize::Graphics::Shader();
            fragment_shader = Voxelize::Graphics::Shader();
            opengl_log = "";
            opengl_log_length = 0;
            opengl_error_code = GL_NO_ERROR;
        }

        // open shaders
        void OpenShaders(Voxelize::Error* _error, std::string _vertex_shader, std::string _fragment_shader) {
            // compile vertex shader
            vertex_shader.OpenShader(_vertex_shader, _error, GL_VERTEX_SHADER);
            if ((*_error).occured) {
                return;
            }

            // compile fragment shader
            fragment_shader.OpenShader(_fragment_shader, _error, GL_FRAGMENT_SHADER);
            if ((*_error).occured) {
                return;
            }

            // create shader program
            program_ID = glCreateProgram();

            // setup shader linking
            glAttachShader(program_ID, vertex_shader.shader_ID);
            glAttachShader(program_ID, fragment_shader.shader_ID);

            // link shaders
            glLinkProgram(program_ID);

            // check for opengl errors
            glGetProgramiv(program_ID, GL_INFO_LOG_LENGTH, &opengl_log_length);
            if (opengl_log_length > 0) {
                // create place for opengl to dump log
                Voxelize::Character log[Voxelize::Graphics::OpenGLErrorLogMaxLength];
                
                // get opengl error data
                glGetProgramInfoLog(program_ID, Voxelize::Graphics::OpenGLErrorLogMaxLength, NULL, (GLchar*)&log);
                opengl_error_code = glGetError();

                // convert log error to string
                opengl_log = std::string((char*)&log, opengl_log_length);

                // generate Voxelize error
                *_error = Voxelize::Error(true, "\"error\": {\n\t\"reason\": \"OpenGL shaders did not link together.\",\n\t\"OpenGL Error Code\": " + std::to_string((int)opengl_error_code) + ",\n\t\"OpenGL Log\": \"" + opengl_log + "\"\n}");
            }
        }

        // use shader program
        void UseProgram() {
            glUseProgram(program_ID);
        }

        // close shaders
        void CloseShaders() {
            // close individual shaders
            vertex_shader.CloseShader();
            fragment_shader.CloseShader();

            // close shader program
            glDeleteProgram(program_ID);
        }
    };
}
