#pragma once

#include "core.h"

class UI {
public:
	UI(const glm::vec2& resolution);
	~UI();

	void begin();
	void end();

private:
	glm::vec2 m_res;
	Camera* m_camera;
};
