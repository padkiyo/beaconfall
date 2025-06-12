#include "error.h"

void gl_clear_error() {
	while(glGetError() != GL_NO_ERROR);
}

void gl_get_error(const char* function, const char* file, int line) {
	GLenum error;
	while ((error = glGetError())) {
		log_error("%s:%d: [%s] (%u -> %s)\n", file, line, function, error, gluErrorString(error));
		exit(1);
	}
}
