#include "frame_controller.h"

FrameController fc_create(i32 fps) {
	return (FrameController) {
		.start_time = SDL_GetTicks(),
		.start_tick = SDL_GetTicks(),
		.unit_frame = 1.0f / fps,
		.dt = 0,
		.frame = 0,
		.fps = 0
	};
}

void fc_start(FrameController* fc) {
	fc->start_tick = SDL_GetTicks();
}

void fc_end(FrameController* fc) {
	// Counting the frames for fps
	fc->frame++;

	fc->dt = (SDL_GetTicks() - fc->start_tick) / 1000.0f; // Converting to seconds

	// If the time between frame (dt) is smaller then unit frame (1000/fps) then sleep for remaining amount of time
	if (fc->unit_frame > fc->dt) {
		u32 secs = fc->unit_frame - fc->dt;
		sleep(secs);
	}

	// Adjust the dt after the sleep
	fc->dt = (SDL_GetTicks() - fc->start_tick) / 1000.0f;

	// If a second is passed then set the counted frame as fps and count again
	if (SDL_GetTicks() - fc->start_time >= 1000.0f) {
		fc->fps = fc->frame;
		fc->start_time = SDL_GetTicks();
		fc->frame = 0;
	}
}
