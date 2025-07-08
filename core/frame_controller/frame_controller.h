#pragma once
#include "common.h"

// NOTE(slok): The delta time is in seconds (eg: 0.016)

class FrameController {
public:
	FrameController(i32 fps);
	~FrameController();

	f64 dt()  const { return m_dt;  }
	i32 fps() const { return m_fps; }

	void begin();
	void end();

private:
	u32 m_start_time, m_start_tick;
	f64 m_unit_frame, m_dt;
	i32 m_frame_cnt, m_fps;
};
