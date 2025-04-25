#pragma once

#include "common.h"

// Gives the size of opengl types
size_t sizeof_gl_type(GLenum gl_type);

// Flips the sdl surface
void flip_surface(SDL_Surface* surface);

// Gives the color format of a surface
u32 get_color_format(SDL_Surface* surface);

// Packing the sdl surface
// reference: https://discourse.libsdl.org/t/sdl-ttf-2-0-18-surface-to-opengl-texture-not-consistent-with-ttf-2-0-15/34529/5
void pack_surface(SDL_Surface* surface);
