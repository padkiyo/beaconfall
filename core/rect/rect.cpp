#include "rect.h"

b32 Rect::intersect(const Rect& r) const {
	return (
		x < r.x + r.w &&
		x + w > r.x &&
		y < r.y + r.h &&
		y + h > r.y
	);
}

b32 Rect::intersect_inclusive(const Rect& r) const {
	return (
		x <= r.x + r.w &&
		x + w >= r.x &&
		y <= r.y + r.h &&
		y + h >= r.y
	);
}

b32 Rect::intersect_point(const glm::vec2& p) const {
	return (
		(x < p.x && p.x < x + w) &&
		(y < p.y && p.y < y + h)
	);
}

void Rect::resolve_x(const Rect& rect, const glm::vec2& movement) {
	m_left = m_right = false;

	if (intersect(rect)) {
		if (movement.x > 0) {
			x = rect.x - w;
			m_right = true;
		} else if (movement.x < 0) {
			x = rect.x + rect.w;
			m_left = true;
		}
	}
}

void Rect::resolve_y(const Rect& rect, const glm::vec2& movement) {
	m_up = m_down = false;

	if (intersect(rect)) {
		if (movement.y > 0) {
			y = rect.y - h;
			m_up = true;
		} else if (movement.y < 0) {
			y = rect.y + rect.h;
			m_down = true;
		}
	}
}
