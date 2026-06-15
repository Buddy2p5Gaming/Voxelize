#pragma once

/* Include */
// C++
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// external
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>

/* Basics */
namespace Voxelize {
    // basic types
    typedef uint8_t U8;
    typedef uint16_t U16;
    typedef uint32_t U32;
    typedef uint64_t U64;
    typedef int8_t S8;
    typedef int16_t S16;
    typedef int32_t S32;
    typedef int64_t S64;
    typedef float F32;
    typedef double F64;
    typedef void* Address;
    typedef bool Boolean;

    // character defines
    typedef Voxelize::U8 Character;

    // generic defines
    typedef Voxelize::U64 Length;

    // error
    class Error {
    public:
        bool occured;
        std::string json;

        // constructors
        Error() {
            occured = false;
            json = "\"error\": {\n\t\"reason\": \"No problem was detected.\"\n}";
        }
        Error(bool _occured, std::string _json) {
            occured = _occured;
            json = _json;
        }
    };

    // buffer
    class Buffer {
    public:
        Voxelize::Address start;
        Voxelize::Address end;

        // constructors
        Buffer() {
            start = 0;
            end = 0;
        }
        Buffer(Voxelize::Address _start, Voxelize::Address _end) {
            start = _start;
            end = _end;
        }

        // length of buffer
        Voxelize::Length Length() {
            return (Voxelize::Length)((Voxelize::U64)end - (Voxelize::U64)start + 1);
        }

        // open
        void Allocate(Voxelize::Length _allocation_size) {
            start = malloc(_allocation_size);
            if (start != 0) {
                end = (Voxelize::Address)((Voxelize::U64)start + _allocation_size - 1);
            } else {
                end = 0;
            }
        }

        // close
        void Deallocate() {
            if (start != 0) {
                free(start);
            }
        }

        // check if buffer is empty
        Voxelize::Boolean IsEmpty() {
            return start == 0 && end == 0;
        }
    };

    class Current {
    public:
        Voxelize::Buffer range;
        Voxelize::Address progress;

        Current() {
            range = Voxelize::Buffer();
            progress = 0;
        }
        Current(Voxelize::Buffer _range, Voxelize::Address _progress) {
            range = _range;
            progress = _progress;
        }
    };

    // file loader
    class FileLoader {
    public:
        Voxelize::Error error;

        // load file into std::string
        std::string LoadTextFile(std::string _file_path) {
            // null init error
            error = Voxelize::Error();

            // open file stream
            std::ifstream file(_file_path);

            // check for error
            if (!file) {
                // setup error
                error = Voxelize::Error(true, "\"error\": {\n\t\"reason\": \"Text file could not be loaded / found.\",\n\t\"file_path\": \"" + _file_path + "\"\n}");

                return "";
            }

            // read file into string
            std::ostringstream stringstream;
            stringstream << file.rdbuf();
            return stringstream.str();
        }
    };

    // world data
    namespace World {
        typedef Voxelize::F32 Axis;
        typedef Voxelize::World::Axis X;
        typedef Voxelize::World::Axis Y;
        typedef Voxelize::World::Axis Z;
    }
}
