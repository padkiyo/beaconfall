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

void Rect::resolve(const std::vector<Rect>& rects, const glm::vec2& movement, f64 dt) {
	m_left = m_right = m_up = m_down = false;

	// X axis resolution
	x += movement.x * dt;

	for (auto r : rects) {
		if (intersect(r)) {
			if (movement.x > 0) {
				x = r.x - w;
				m_right = true;
			} else if (movement.x < 0) {
				x = r.x + r.w;
				m_left = true;
			}
		}
	}

	// Y axis resolution
	y += movement.y * dt;

	for (auto r : rects) {
		if (intersect(r)) {
			if (movement.y > 0) {
				y = r.y - h;
				m_up = true;
			} else if (movement.y < 0) {
				y = r.y + r.h;
				m_down = true;
			}
		}
	}
}
