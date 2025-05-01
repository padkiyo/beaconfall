#pragma once

#include "core.h"

void map_entry(void* data);
void map_exit(void*data);
void map_update(void*data, f64 dt);
void map_event(void* data, SDL_Event event, f64 dt);
