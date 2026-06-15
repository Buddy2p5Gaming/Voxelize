#pragma once

// Voxelize
#include "basic.hpp"
#include "window.hpp"
#include "shaders.hpp"
#include <SDL3/SDL_events.h>

/* Game Loop */
namespace Voxelize {
    class Game {
        // game state
        Voxelize::Graphics::Window gameWindow;
        Voxelize::Boolean isRunning;

        // shaders
        Voxelize::Graphics::Shaders chunkShaders;

        // file loaders
        Voxelize::FileLoader fileLoader;

    public:
        // error
        Voxelize::Error error;

        Game() {
            // game state
            gameWindow = Voxelize::Graphics::Window();
            isRunning = false;

            // shaders
            chunkShaders = Voxelize::Graphics::Shaders();

            // file loaders
            fileLoader = Voxelize::FileLoader();

            // error
            error = Voxelize::Error();
        }

    private:
        // compile shader program
        Voxelize::Graphics::Shaders CompileShaders(std::string vertexShaderFilePath, std::string fragmentShaderFilePath) {
            Voxelize::Graphics::Shaders output;

            // load files
            std::string vertex = fileLoader.LoadTextFile(vertexShaderFilePath);
            if (fileLoader.error.occured) {
                error = fileLoader.error;
                return Voxelize::Graphics::Shaders();
            }
            std::string fragment = fileLoader.LoadTextFile(fragmentShaderFilePath);
            if (fileLoader.error.occured) {
                error = fileLoader.error;
                return Voxelize::Graphics::Shaders();
            }

            // compile shaders
            output.OpenShaders(&error, vertex, fragment);
            return output;
        }

    public:
        // game loop
        void Run() {
            // game start
            std::cout << "Running new game!" << std::endl;

            // force x11 over wayland
            setenv("SDL_VIDEODRIVER", "x11", 1);

            // init SDL3
            if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
                // setup error
                error = Voxelize::Error(true, "\"error\": {\"reason\": \"SDL3 did not initialize.\"}");

                printf("%s", (const char*)SDL_GetError());
                printf("\n");

                return;
            }

            // start runner
            isRunning = true;

            // open game Window
            gameWindow = Voxelize::Graphics::Window();
            error = gameWindow.Open(Voxelize::Graphics::WindowStyling("Voxelize", 800, 600));
            if (error.occured) {
                return;
            }

            // open shaders
            chunkShaders = CompileShaders("./source/shaders/chunks/vertex.glsl", "./source/shaders/chunks/fragment.glsl");
            if (error.occured) {
                return;
            }

            // temp event
            SDL_Event tempEvent;

            // game loop
            while (isRunning) {
                // check for quit
                while (SDL_PollEvent(&tempEvent)) {
                    switch (tempEvent.type) {
                    // in case of game stop
                    case SDL_EVENT_QUIT:
                        // stop game
                        isRunning = false;

                        break;
                    // in case of Window resize
                    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
                        glViewport(0, 0, tempEvent.window.data1, tempEvent.window.data2);

                        break;
                    default:
                        break;
                    }
                }

                // next frame
                gameWindow.NextFrame();
            }

            // close game
            gameWindow.Close();
            SDL_Quit();

            // thank player
            std::cout << "Thanks for playing Voxelize!" << std::endl;

            return;
        }
    };
}
