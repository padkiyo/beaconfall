#pragma once

#include "core.h"
#include "entity/entity.h"

class Gem : public Entity {
public:
	Gem(std::vector<Entity*>& entities);
	~Gem();

	void render(const SpriteManager& sprt_mgr, std::vector<Quad>& quads);

private:
	std::vector<Entity*>& m_entities;
};
