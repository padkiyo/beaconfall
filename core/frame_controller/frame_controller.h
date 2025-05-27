#pragma once
#include "common.h"

// NOTE: The time is in miliseconds
struct FrameController {
	u32 start_time;
	u32 start_tick;
	f64 unit_frame;
	f64 dt;
	i32 frame;
	i32 fps;
};

FrameController fc_create(i32 fps);
void fc_start(FrameController* fc);
void fc_end(FrameController* fc);
