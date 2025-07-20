#pragma once

#include "core.h"
#include "entity/entity.h"

enum GemType {
	GEM_POWER,
	GEM_EXP,
};

class Gem : public Entity {
public:
	GemType gem_type;

public:
	Gem(std::vector<Entity*>& entities, GemType gem_type);
	~Gem();

	f32 get_value();
	void render(const SpriteManager& sprt_mgr, std::vector<Quad>& quads, std::vector<Light>& lights);

private:
	std::vector<Entity*>& m_entities;
};
