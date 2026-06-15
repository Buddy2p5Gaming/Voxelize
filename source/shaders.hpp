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
        GLuint shaderID;
        std::string code;

        // error data
        std::string OpenGLLog;
        GLint OpenGLLogLength;
        GLenum OpenGLErrorCode;

        // constructors
        Shader() {
            shaderID = 0;
            code = "";
            OpenGLLog = "";
            OpenGLLogLength = 0;
            OpenGLErrorCode = GL_NO_ERROR;
        }

        // open and compile shader
        void OpenShader(std::string _code, Voxelize::Error* _error, GLenum _shaderType) {
            // create new shader
            shaderID = glCreateShader(_shaderType);

            // setup temp
            GLchar* temp = (GLchar*)_code.c_str();

            // send source code to gpu
            glShaderSource(shaderID, 1, (const GLchar* const*)&temp, NULL);

            // compiler the shader
            glCompileShader(shaderID);

            // check for OpenGL errors
            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &OpenGLLogLength);
            if (OpenGLLogLength > 0) {
                // create place for OpenGL to dump log
                Voxelize::Character log[Voxelize::Graphics::OpenGLErrorLogMaxLength];

                // get OpenGL error data
                glGetShaderInfoLog(shaderID, Voxelize::Graphics::OpenGLErrorLogMaxLength, NULL, (GLchar*)&log);
                OpenGLErrorCode = glGetError();

                // convert log error to string
                OpenGLLog = std::string((char*)&log, OpenGLLogLength);

                // generate Voxelize error
                if (_shaderType == GL_VERTEX_SHADER) {
                    *_error = Voxelize::Error(true, "\"error\": {\n\t\"reason\": \"OpenGL vertex shader did not compile.\",\n\t\"OpenGL Error Code\": " + std::to_string((int)OpenGLErrorCode) + ",\n\t\"OpenGL Log\": \"" + OpenGLLog + "\"\n}");
                } else if (_shaderType == GL_FRAGMENT_SHADER) {
                    *_error = Voxelize::Error(true, "\"error\": {\n\t\"reason\": \"OpenGL fragment shader did not compile.\",\n\t\"OpenGL Error Code\": " + std::to_string((int)OpenGLErrorCode) + ",\n\t\"OpenGL Log\": \"" + OpenGLLog + "\"\n}");
                }
            }

            return;
        }

        // close shader
        void CloseShader() {
            glDeleteShader(shaderID);
        }
    };

    // one shader program
    class Shaders {
    public:
        // shader program data
        GLuint programID;
        Voxelize::Graphics::Shader vertexShader;
        Voxelize::Graphics::Shader fragmentShader;

        // error data
        std::string OpenGLLog;
        GLint OpenGLLoglength;
        GLenum OpenGLErrorCode;

        // constructors
        Shaders() {
            programID = 0;
            vertexShader = Voxelize::Graphics::Shader();
            fragmentShader = Voxelize::Graphics::Shader();
            OpenGLLog = "";
            OpenGLLoglength = 0;
            OpenGLErrorCode = GL_NO_ERROR;
        }

        // open shaders
        void OpenShaders(Voxelize::Error* _error, std::string _vertexShader, std::string _fragmentShader) {
            // compile vertex shader
            vertexShader.OpenShader(_vertexShader, _error, GL_VERTEX_SHADER);
            if ((*_error).occured) {
                return;
            }

            // compile fragment shader
            fragmentShader.OpenShader(_fragmentShader, _error, GL_FRAGMENT_SHADER);
            if ((*_error).occured) {
                return;
            }

            // create shader program
            programID = glCreateProgram();

            // setup shader linking
            glAttachShader(programID, vertexShader.shaderID);
            glAttachShader(programID, fragmentShader.shaderID);

            // link shaders
            glLinkProgram(programID);

            // check for OpenGL errors
            glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &OpenGLLoglength);
            if (OpenGLLoglength > 0) {
                // create place for OpenGL to dump log
                Voxelize::Character log[Voxelize::Graphics::OpenGLErrorLogMaxLength];
                
                // get OpenGL error data
                glGetProgramInfoLog(programID, Voxelize::Graphics::OpenGLErrorLogMaxLength, NULL, (GLchar*)&log);
                OpenGLErrorCode = glGetError();

                // convert log error to string
                OpenGLLog = std::string((char*)&log, OpenGLLoglength);

                // generate Voxelize error
                *_error = Voxelize::Error(true, "\"error\": {\n\t\"reason\": \"OpenGL shaders did not link together.\",\n\t\"OpenGL Error Code\": " + std::to_string((int)OpenGLErrorCode) + ",\n\t\"OpenGL Log\": \"" + OpenGLLog + "\"\n}");
            }
        }

        // use shader program
        void UseProgram() {
            glUseProgram(programID);
        }

        // close shaders
        void CloseShaders() {
            // close individual shaders
            vertexShader.CloseShader();
            fragmentShader.CloseShader();

            // close shader program
            glDeleteProgram(programID);
        }
    };
}
