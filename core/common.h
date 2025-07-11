#pragma once

// Windows check
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #include<windows.h>

    //! Scary
    #undef near
    #undef far
    
    #define sleep Sleep
    #define SDL_MAIN_HANDLED
#endif

#include <glad/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>
#include <stb_image.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/constants.hpp>
#include <json/json.h>

#include <unordered_map>
#include <string>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <format>
#include <variant>

#include "base/types.h"
#include "base/result.h"
#include "base/log.h"
#include "base/utils.h"
#include "base/error.h"
