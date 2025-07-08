#include "rect.h"

b32 Rect::intersect(const Rect& r) {
	return (
		((x < r.x && r.x < x + w)        ||
		 (r.x < x && x < r.x + r.w)      ||
		 (x < r.x && r.x + r.w < x + w)  ||
		 (r.x < x && x + w < r.x + r.w)) &&
		((y < r.y && r.y < y + h)        ||
		 (r.y < y && y < r.y + r.h)      ||
		 (y < r.y && r.y + r.h < y + h)  ||
		 (r.y < y && y + h < r.y + r.h))
	);
}

b32 Rect::intersect_inclusive(const Rect& r) {
	return (
		((x <= r.x && r.x <= x + w)        ||
		 (r.x <= x && x <= r.x + r.w)      ||
		 (x <= r.x && r.x + r.w <= x + w)  ||
		 (r.x <= x && x + w <= r.x + r.w)) &&
		((y <= r.y && r.y <= y + h)        ||
		 (r.y <= y && y <= r.y + r.h)      ||
		 (y <= r.y && r.y + r.h <= y + h)  ||
		 (r.y <= y && y + h <= r.y + r.h))
	);
}

b32 Rect::intersect_point(const glm::vec2& p) {
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
				f32 dx = x + w - r.x;
				x -= dx;

				m_right = true;
			} else if (movement.x < 0) {
				f32 dx = r.x + r.w - x;
				x += dx;

				m_left = true;
			}
		}
	}

	// Y axis resolution
	y += movement.y * dt;

	for (auto r : rects) {
		if (intersect(r)) {
			if (movement.y > 0) {
				f32 dy = y + h - r.y;
				y -= dy;

				m_up = true;
			} else if (movement.y < 0) {
				f32 dy = r.y + r.h - y;
				y += dy;

				m_down = true;
			}
		}
	}
}
