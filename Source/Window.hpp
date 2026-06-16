#pragma once

/* Include */
// Voxelize
#include "Basic.hpp"

/* Window */
namespace Voxelize::Graphics {
    // window defines
    typedef Voxelize::U64 WindowWidth;
    typedef Voxelize::U64 WindowHeight;

    // window appearance data (NOT including inner window, opengl takes care of that)
    class WindowStyling {
    public:
        // window about
        std::string title;
        Voxelize::Graphics::WindowWidth width;
        Voxelize::Graphics::WindowHeight height;

        // constructors
        WindowStyling() {
            title = "Default Window";
            width = 800;
            height = 600;
        }
        WindowStyling(std::string _title, Voxelize::Graphics::WindowWidth _width, Voxelize::Graphics::WindowHeight _height) {
            title = _title;
            width = _width;
            height = _height;
        }

        // update size (DOES NOT UPDATE OPENGL, just updates the values)
        void UpdateWindowSize(Voxelize::Graphics::WindowWidth _width, Voxelize::Graphics::WindowHeight _height) {
            width = _width;
            height = _height;
        }
    };

    // window
    class Window {
    public:
        SDL_Window* windowContext;
        SDL_GLContext SDL3OpenGLContext;
        Voxelize::Graphics::WindowStyling windowStyling;

        Window() {
            windowContext = 0;
            SDL3OpenGLContext = 0;
            windowStyling = Voxelize::Graphics::WindowStyling();
        }

        // open window
        Voxelize::Error Open(Voxelize::Graphics::WindowStyling style) {
            // setup no error
            Voxelize::Error output = Voxelize::Error();

            // set styling
            windowStyling = style;

            // setup opengl expectations
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

            // initalize window
            windowContext = SDL_CreateWindow((const char*)style.title.c_str(), style.width, style.height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
            if (windowContext == 0) {
                // return error
                return Voxelize::Error(true, "\"error\": {\"reason\": \"Failed to create SDL3 window context.\"}");
            }

            // initalize opengl context
            SDL3OpenGLContext = SDL_GL_CreateContext(windowContext);
            if (SDL3OpenGLContext == 0) {
                // quit cleanly
                SDL_DestroyWindow(windowContext);

                // return error
                return Voxelize::Error(true, "\"error\": {\"reason\": \"Failed to create OpenGL context.\"}");
            }

            // initalize glew
            glewExperimental = GL_TRUE;
            GLenum glewError = glewInit();
            if (glewError != GLEW_OK) {
                // quit cleanly
                SDL_GL_DestroyContext(SDL3OpenGLContext);
                SDL_DestroyWindow(windowContext);

                // return error
                return Voxelize::Error(true, "\"error\": {\"reason\": \"GLEW did not initialize.\"}");
            }

            // setup clear color
            glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

            // window initilaized
            return output;
        }

        // close window
        void Close() {
            // close Graphics
            SDL_GL_DestroyContext(SDL3OpenGLContext);
            SDL_DestroyWindow(windowContext);
        }

        // setup at loop end to setup next frame
        void NextFrame() {
            // display window data
            SDL_GL_SwapWindow(windowContext);

            // clear buffer
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        // update window size
        void UpdateWindowSize(Voxelize::Graphics::WindowWidth _width, Voxelize::Graphics::WindowHeight _height) {
            // update stats
            windowStyling.UpdateWindowSize(_width, _height);

            // update OpenGL
            glViewport(0, 0, (GLsizei)_width, (GLsizei)_height);
        }
    };
}
