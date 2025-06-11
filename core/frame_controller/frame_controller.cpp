#include "frame_controller.h"

FrameController::FrameController(i32 fps) {
	m_start_time = SDL_GetTicks();
	m_start_tick = SDL_GetTicks();
	m_unit_frame = 1.0 / fps;
	m_dt = 0;
	m_frame_cnt = 0;
	m_fps = 0;
}

FrameController::~FrameController() {
}

void FrameController::begin() {
	m_start_tick = SDL_GetTicks();
}

void FrameController::end() {
	// Counting the frames for fps
	m_frame_cnt++;

	m_dt = (SDL_GetTicks() - m_start_tick) / 1000.0; // Converting to seconds

	// If the time between frame (dt) is smaller then unit frame (1000/fps) then sleep for remaining amount of time
	if (m_unit_frame > m_dt) {
		u32 secs = m_unit_frame - m_dt;
		sleep(secs);
	}

	// Adjust the dt after the sleep
	m_dt = (SDL_GetTicks() - m_start_tick) / 1000.0;

	// If a second is passed then set the counted frame as fps and count again
	if (SDL_GetTicks() - m_start_time >= 1000) {
		m_fps = m_frame_cnt;
		m_start_time = SDL_GetTicks();
		m_frame_cnt = 0;
	}
}
