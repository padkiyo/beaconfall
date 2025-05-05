#pragma once

#include "core.h"


void notebook_entry(void* data);
void notebook_exit(void* data);
void notebook_update(void* data, f64 dt);
void notebook_event(void* data, SDL_Event event, f64 dt);
