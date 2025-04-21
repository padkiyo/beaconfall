#include "error.h"

void gl_clear_error(){
	while(glGetError() != GL_NO_ERROR){
	}
}

void gl_get_error(const char* function, const char* file, int line){
	GLenum error = glGetError();

	// ! GluErrorString() is deprecated in GLAD
	// So Only prints our error the error code
	// TODO Find a alternative
	while(error){
		printf("[ERROR] %s:%s:%d %u", function, file, line, error);
	}
}
