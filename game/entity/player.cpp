#include "player.h"

Player::Player(std::vector<Entity*>& entities, const Camera& camera)
	: Entity(ENT_PLAYER), m_entities(entities), m_camera(camera) {
	m_rect.x = 0;
	m_rect.y = 0;
	m_rect.w = 32;
	m_rect.h = 32;

	m_health = 100.0f;

	// Interactions
	m_reach_area.w = 120;
	m_reach_area.h = 120;
	m_feed_beacon = false;

	// Pickup
	m_picked_item = nullptr;

	// Movement
	m_move = { 0, 0 };
	m_speed = 200.0f;
	m_left = m_right = m_up = m_down = false;

	// Attack
	m_can_atk = true;
	m_attack = false;
	m_atk_dmg = 50.0f;
	m_atk_hitbox.w = 64;
	m_atk_hitbox.h = 64;
}

Player::~Player() {
}

void Player::set_speed(f32 speed) {
	m_speed = speed;
}

void Player::handle_event(const SDL_Event& event) {
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
			case SDLK_w:
				m_up = true;
				break;
			case SDLK_s:
				m_down = true;
				break;
			case SDLK_a:
				m_left = true;
				break;
			case SDLK_d:
				m_right = true;
				break;

			// Item pickup
			case SDLK_f:
				handle_item_pickup();
				break;

			// Feed beacon
			case SDLK_e:
				m_feed_beacon = true;
				break;
		}
	}
	else if (event.type == SDL_KEYUP) {
		switch (event.key.keysym.sym) {
			case SDLK_w:
				m_up = false;
				break;
			case SDLK_s:
				m_down = false;
				break;
			case SDLK_a:
				m_left = false;
				break;
			case SDLK_d:
				m_right = false;
				break;

			// Feed beacon
			case SDLK_e:
				m_feed_beacon = false;
				break;
		}
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			if (m_can_atk) {
				m_attack = true;
				m_can_atk = false;
				m_atk_cooldown = 10.0f;
			}
		}
	}
	else if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			m_attack = false;
		}
	}
}

void Player::handle_attack() {
	if (m_attack) {
		i32 x, y;
		SDL_GetMouseState(&x, &y);
		y = WIN_HEIGHT - y;

		// Fixing the mouse position relative to camera
		glm::vec2 cam_pos = m_camera.get_pos();
		x += cam_pos.x;
		y += cam_pos.y;

		calc_atk_hitbox(x,y);

		for (auto ent: m_entities) {
			if (m_atk_hitbox.intersect(ent->rect())) {
				ent->take_damage(m_atk_dmg);
			}
		}
	}

	// Attack cooldown
	if (m_atk_cooldown > 0.0f) {
		m_atk_cooldown -= 0.8f;
	} else {
		m_atk_cooldown = 0.0f;
		m_can_atk = true;
	}
}

void Player::handle_collision(f64 dt) {
	m_rect.x += m_move.x * dt;
	for (auto ent: m_entities) {
		m_rect.resolve_x(ent->rect(), m_move);
	}

	m_rect.y += m_move.y * dt;
	for (auto ent: m_entities) {
		m_rect.resolve_y(ent->rect(), m_move);
	}
}

void Player::handle_item_pickup() {
	b32 picked_new_item = false;

	for (auto ent: m_entities) {
		Rect e_rect = ent->rect();
		if (m_reach_area.intersect(e_rect) && ent != m_picked_item) {
			switch (ent->type) {

				// Gem holding
				case ENT_GEM: {
					// If we already have a item then we swap
					if (m_picked_item) {
						m_picked_item->set_pos({e_rect.x, e_rect.y});
					}

					m_picked_item = ent;
					picked_new_item = true;
				} break;

			}
		}

		if (picked_new_item) break;
	}

	// If we press pick again and there is not any items nearby
	// we drop the current item
	if (!picked_new_item && m_picked_item) {
		Rect r = m_picked_item->rect();
		m_picked_item->set_pos({
			m_rect.x + m_rect.w / 2 - r.w / 2,
			m_rect.y + m_rect.h / 2 - r.h / 2
		});

		// Remove the picked item
		m_picked_item = nullptr;
	}
}

void Player::handle_beacon_interaction(Beacon* beacon) {
	if (!m_feed_beacon) return;
	if (!m_picked_item) return;

	Rect b_rect = beacon->rect();
	if (m_reach_area.intersect(b_rect)) {
		switch (m_picked_item->type) {
			case ENT_GEM: {
				Gem* gem = dynamic_cast<Gem*>(m_picked_item);
				switch (gem->gem_type) {
					case GEM_POWER:
						beacon->add_power(gem->get_value());
						break;
					case GEM_EXP:
						beacon->add_exp(gem->get_value());
						break;
				}
				m_entities.erase(
					std::remove(m_entities.begin(), m_entities.end(), m_picked_item),
					m_entities.end()
				);

				delete m_picked_item;
				m_picked_item = nullptr;
			} break;
		}
	}
}

void Player::update(Beacon* beacon, f64 dt) {
	// Movement
	if (m_left)
		m_move.x -= m_speed;
	if (m_right)
		m_move.x += m_speed;
	if (m_up)
		m_move.y += m_speed;
	if (m_down)
		m_move.y -= m_speed;

	// Calculate reach area per frame
	m_reach_area.x = m_rect.x + m_rect.w / 2 - m_reach_area.w / 2;
	m_reach_area.y = m_rect.y + m_rect.h / 2 - m_reach_area.h / 2;

	handle_attack();
	handle_collision(dt);
	handle_beacon_interaction(beacon);

	// If we have picked the item, carry it in head
	if (m_picked_item) {
		Rect r = m_picked_item->rect();
		m_picked_item->set_pos({
			m_rect.x + m_rect.w / 2 - r.w / 2,
			m_rect.y + m_rect.h
		});
	}

	// Take damage whenever we're outside of the zone
	Rect b_rect = beacon->rect();
	glm::vec2 c1 = {
		m_rect.x + m_rect.w / 2,
		m_rect.y + m_rect.h / 2
	};
	glm::vec2 c2 = {
		b_rect.x + b_rect.w / 2,
		b_rect.y + b_rect.h / 2
	};

	if (
		glm::distance(c1, c2) < beacon->get_radius() &&   // Inside range
		beacon->get_power() > 0.0f                        // Beacon got power
	) {
		m_health += 1.0f;
		if (m_health >= 100.0f)
			m_health = 100.0f;
	} else {
		take_damage(1.0f);
	}

	// Reset the movement
	m_move = {0,0};
}

void Player::render(const SpriteManager& sprt_mgr, std::vector<Quad>& quads, std::vector<Light>& lights) {
	if (m_attack) {
		quads.push_back(Quad {
			{ m_atk_hitbox.x, m_atk_hitbox.y, 0 },
			{ m_atk_hitbox.w, m_atk_hitbox.h },
			glm::mat4(1),
			nullptr,
			{ 0, 0, 1, 1 },
			{ 0, 1, 0, 0.8 }
		});

		// Reset the attack
		m_attack = false;
	}

	quads.push_back(Quad {
		.pos = {m_reach_area.x, m_reach_area.y, 0},
		.size = {m_reach_area.w, m_reach_area.h},
		.rot = glm::mat4(1),
		.texture = nullptr,
		.uv = {0, 0, 1, 1},
		.color = {1,1,0,0.5},
	});

	// Handling damage effect
	if (is_stunned() && m_health < 50.0f && m_stun > m_stun_timeout / 2) {
		m_overlay.a = 1.0f;
	} else {
		m_overlay.a = 0.0f;
	}

	// Rendering the player
	quads.push_back(Quad {
		.pos = {m_rect.x, m_rect.y, 0},
		.size = {m_rect.w, m_rect.h},
		.rot = glm::mat4(1),
		.texture = &sprt_mgr.get_spritesheet_texture(PLAYER),
		.uv = {0, 0, 1, 1},
		.color = {1,1,1,1},
		.overlay = m_overlay
	});
}

void Player::calc_atk_hitbox(i32 x, i32 y) {
	// left
	if (x < m_rect.x) {
		// bottom left
		if (y < m_rect.y) {
			m_atk_hitbox.x = m_rect.x + m_rect.w / 2 - m_atk_hitbox.w;
			m_atk_hitbox.y = m_rect.y + m_rect.h / 2 - m_atk_hitbox.h;
		}
		// top left
		if (m_rect.y + m_rect.h < y) {
			m_atk_hitbox.x = m_rect.x + m_rect.w / 2 - m_atk_hitbox.w;
			m_atk_hitbox.y = m_rect.y + m_rect.h / 2;
		}
		// mid left
		if (m_rect.y < y && y < m_rect.y + m_rect.h) {
			m_atk_hitbox.x = m_rect.x + m_rect.w / 2 - m_atk_hitbox.w;
			m_atk_hitbox.y = m_rect.y + m_rect.h / 2 - m_atk_hitbox.h / 2;
		}
	}

	// right
	else if (m_rect.x + m_rect.w < x) {
		// bottom right
		if (y < m_rect.y) {
			m_atk_hitbox.x = m_rect.x + m_rect.w / 2;
			m_atk_hitbox.y = m_rect.y + m_rect.h / 2 - m_atk_hitbox.h;
		}
		// top right
		if (m_rect.y + m_rect.h < y) {
			m_atk_hitbox.x = m_rect.x + m_rect.w / 2;
			m_atk_hitbox.y = m_rect.y + m_rect.h / 2;
		}
		// mid right
		if (m_rect.y < y && y < m_rect.y + m_rect.h) {
			m_atk_hitbox.x = m_rect.x + m_rect.w / 2;
			m_atk_hitbox.y = m_rect.y + m_rect.h / 2 - m_atk_hitbox.h / 2;
		}
	}

	// mid
	else if (m_rect.x < x && x < m_rect.x + m_rect.w) {
		// top
		if (m_rect.y < y) {
			m_atk_hitbox.x = m_rect.x + m_rect.w / 2 - m_atk_hitbox.w / 2;
			m_atk_hitbox.y = m_rect.y + m_rect.h / 2;
		}
		// bottom
		if (y < m_rect.y) {
			m_atk_hitbox.x = m_rect.x + m_rect.w / 2 - m_atk_hitbox.w / 2;
			m_atk_hitbox.y = m_rect.y + m_rect.h / 2 - m_atk_hitbox.h;
		}
	}
}
