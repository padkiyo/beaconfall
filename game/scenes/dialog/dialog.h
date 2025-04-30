#pragma once

#include "core.h"

void dialog_entry(void* data);
void dialog_exit(void* data);
void dialog_update(void* data, f64 dt);
void dialog_event(void* data, SDL_Event event, f64 dt);
