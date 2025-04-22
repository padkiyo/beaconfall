#pragma once

#include "common.h"

void imgui_create(SDL_Window* sdl_window, SDL_GLContext gl_context);
void imgui_destroy();
void imgui_begin_frame();
void imgui_end_frame();
