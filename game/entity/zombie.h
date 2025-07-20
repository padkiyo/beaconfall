#pragma once

#include "core.h"
#include "entity.h"
#include "systems/sprite_system/sprite_system.h"
#include "systems/animation_system/animation_system.h"
#include "entity/player.h"
#include "structure/beacon.h"

#define ZOMBIE_SPEED 40.0f // units travel per sec
#define ZOMBIE_ATTACK_RATE 10.0f
#define ZOMBIE_VISION_RANGE 200.0f
#define ZOMBIE_DAMAGE 10.0f
													 //
enum zombie_state {
	SEARCH_BEACON,
	SEARCH_PLAYER,
	ATTACK_BEACON,
	ATTACK_PLAYER
};

class Zombie : public Entity {
public:
	Zombie(Player* player, Beacon* beacon, std::vector<Entity*>& entities, FrameController* fc);
	~Zombie();

	void on_damage();
	void render(const SpriteManager& sprt_mgr, std::vector<Quad>& quads, std::vector<Light>& lights);
	void update_ai(Beacon* beacon);
	void update(f64 dt);

	void handle_collision(f64 dt);

private:
	Rect* zombie_vision;
	zombie_state current_ai_state = SEARCH_BEACON;
	FrameController* fc;
	Beacon* beacon;
	Player* player;
	glm::vec2 m_move;
	f32 start_time = SDL_GetTicks();
	SpriteManager* m_sprt_mgr;
	Animator* animator;
	std::vector<Entity*>& m_entities;
	glm::vec4 m_overlay = {1, 1, 1, 0};
};

