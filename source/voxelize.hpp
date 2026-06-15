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
        Voxelize::Graphics::Window game_window;
        Voxelize::Boolean is_running;

        // shaders
        Voxelize::Graphics::Shaders chunk_shaders;

        // file loaders
        Voxelize::FileLoader file_loader;

    public:
        // error
        Voxelize::Error error;

        Game() {
            // game state
            game_window = Voxelize::Graphics::Window();
            is_running = false;

            // shaders
            chunk_shaders = Voxelize::Graphics::Shaders();

            // file loaders
            file_loader = Voxelize::FileLoader();

            // error
            error = Voxelize::Error();
        }

    private:
        // compile shader program
        Voxelize::Graphics::Shaders CompileShaders(std::string vertex_shader_file_path, std::string fragment_shader_file_path) {
            Voxelize::Graphics::Shaders output;

            // load files
            std::string vertex = file_loader.LoadTextFile(vertex_shader_file_path);
            if (file_loader.error.occured) {
                error = file_loader.error;
                return Voxelize::Graphics::Shaders();
            }
            std::string fragment = file_loader.LoadTextFile(fragment_shader_file_path);
            if (file_loader.error.occured) {
                error = file_loader.error;
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
            is_running = true;

            // open game Window
            game_window = Voxelize::Graphics::Window();
            error = game_window.Open(Voxelize::Graphics::WindowStyling("Voxelize", 800, 600));
            if (error.occured) {
                return;
            }

            // open shaders
            chunk_shaders = CompileShaders("./source/shaders/chunks/vertex.glsl", "./source/shaders/chunks/fragment.glsl");
            if (error.occured) {
                return;
            }

            // temp event
            SDL_Event temp_event;

            // game loop
            while (is_running) {
                // check for quit
                while (SDL_PollEvent(&temp_event)) {
                    switch (temp_event.type) {
                    // in case of game stop
                    case SDL_EVENT_QUIT:
                        // stop game
                        is_running = false;

                        break;
                    // in case of Window resize
                    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
                        glViewport(0, 0, temp_event.window.data1, temp_event.window.data2);

                        break;
                    default:
                        break;
                    }
                }

                // next frame
                game_window.NextFrame();
            }

            // close game
            game_window.Close();
            SDL_Quit();

            // thank player
            std::cout << "Thanks for playing Voxelize!" << std::endl;

            return;
        }
    };
}
