#include "utils.h"

size_t sizeof_gl_type(GLenum gl_type) {
	size_t sz = 0;
	switch (gl_type) {
		case GL_FLOAT        : sz = sizeof(f32); break;
		case GL_INT          : sz = sizeof(i32); break;
		case GL_UNSIGNED_INT : sz = sizeof(u32); break;
		case GL_UNSIGNED_BYTE: sz = sizeof(u8); break;
		default: panic(0, "Unhandled gl type", gl_type); break;
	}
	return sz;
}

void flip_surface(SDL_Surface* surface)  {
	SDL_LockSurface(surface);

	i32 pitch = surface->pitch; // row size
	char temp[pitch];
	char* pixels = (char*) surface->pixels;

	for(i32 i = 0; i < surface->h / 2; ++i) {
		// get pointers to the two rows to swap
		char* row1 = pixels + i * pitch;
		char* row2 = pixels + (surface->h - i - 1) * pitch;

		// swap rows
		memcpy(temp, row1, pitch);
		memcpy(row1, row2, pitch);
		memcpy(row2, temp, pitch);
	}
	SDL_UnlockSurface(surface);
}

u32 get_color_format(SDL_Surface* surface) {
	u32 colors = surface->format->BytesPerPixel;
	u32 format;

	if (colors == 4) {   // alpha
		if (surface->format->Rmask == 0x000000ff)
			format = GL_RGBA;
		else
			format = GL_BGRA;
	} else {             // no alpha
		if (surface->format->Rmask == 0x000000ff)
			format = GL_RGB;
		else
			format = GL_BGR;
	}
	return format;
}

void pack_surface(SDL_Surface* surface) {
	i32 i;
	u32 len = surface->w * surface->format->BytesPerPixel;
	u8 *src = (Uint8*) surface->pixels;
	u8 *dst = (Uint8*) surface->pixels;
	for (i = 0; i < surface->h; i++) {
		SDL_memmove(dst, src, len);
		dst += len;
		src += surface->pitch;
	}
	surface->pitch = len;
}

