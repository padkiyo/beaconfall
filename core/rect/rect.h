#pragma once

#include "common.h"

class Rect {
public:
	f32 x, y, w, h;

public:
	Rect() {}
	Rect(const glm::vec4& r)
		: x(r.x), y(r.y), w(r.z), h(r.w)
		{}
	~Rect() {};

	b32 intersect(const Rect& r) const;            // Does comparision with <
	b32 intersect_inclusive(const Rect& r) const;  // Does comparision with <=
	b32 intersect_point(const glm::vec2& p) const;
	void resolve_x(const Rect& rect, const glm::vec2& movement);
	void resolve_y(const Rect& rect, const glm::vec2& movement);

	b32 intersect_at_left()  const { return m_left;  }
	b32 intersect_at_right() const { return m_right; }
	b32 intersect_at_up()    const { return m_up;    }
	b32 intersect_at_down()  const { return m_down;  }

	void set_collidable(b32 c) { m_collidable = c; }
	b32 is_collidable() const { return m_collidable; }

private:
	b32 m_left, m_right, m_up, m_down;
	b32 m_collidable = true;
};
