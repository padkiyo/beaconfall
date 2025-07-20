#include "zombie.h"
#include "item/gem.h"

void Zombie::update_ai(Beacon*  beacon) {
	if(this->current_ai_state == SEARCH_BEACON){

		f32 gradx =  beacon->rect().x - m_rect.x;
		f32 grady = beacon->rect().y - m_rect.y;


		glm::vec2 move_dir = glm::normalize(glm::vec2(gradx, grady));


		m_move.x = move_dir.x * (ZOMBIE_SPEED) ;
		m_move.y  = move_dir.y * (ZOMBIE_SPEED);
	}

	if(this->current_ai_state == SEARCH_PLAYER) {

		f32 gradx =  player->rect().x - m_rect.x;
		f32 grady = player->rect().y - m_rect.y;

		glm::vec2 move_dir = glm::normalize(glm::vec2(gradx, grady));

		m_move.x = move_dir.x * (ZOMBIE_SPEED) ;
		m_move.y  = move_dir.y * (ZOMBIE_SPEED);
	}
}

Zombie::Zombie(Player* player, Beacon* beacon, std::vector<Entity*>& entities, FrameController* fc): Entity(ENT_ZOMBIE), m_entities(entities)  {
	m_rect.x = 0;
	m_rect.y = 0;
	m_rect.w = 32;
	m_rect.h = 32;

	this->beacon = beacon;
	this->player = player;
	this->fc = fc;

	this->zombie_vision = new Rect({m_rect.x - m_rect.w/2, m_rect.y - m_rect.h/2, ZOMBIE_VISION_RANGE, ZOMBIE_VISION_RANGE});

	m_health = 100.0f;
	this->animator = new Animator(ZOMBIE, ZOMBIE_WALK);
	this->animator->add_animation(ZOMBIE_WALK, 1000.0f, true);
	this->animator->add_animation(ZOMBIE_ATTACK,1000.0f, true);
}

Zombie::~Zombie() {
}

void Zombie::handle_collision(f64 dt) {

	m_rect.x += m_move.x * dt;
	if (m_rect.intersect(this->beacon->rect()) && this->current_ai_state == SEARCH_BEACON) {
		this->current_ai_state = ATTACK_BEACON;
		this->animator->switch_frame(ZOMBIE_ATTACK);
	}

	for (auto ent: m_entities) {
		if(ent != this)
			m_rect.resolve_x(ent->rect(), m_move);
	}

	m_rect.y += m_move.y * dt;

	if (m_rect.intersect(this->beacon->rect()) && this->current_ai_state == SEARCH_BEACON) {
		this->current_ai_state = ATTACK_BEACON;
		this->animator->switch_frame(ZOMBIE_ATTACK);
	}

	for (auto ent:m_entities){
		if(ent != this)
			m_rect.resolve_y(ent->rect(), m_move);
	}

	if(m_rect.intersect(this->player->rect())) {
			this->current_ai_state = ATTACK_PLAYER;
			this->player->take_damage(ZOMBIE_DAMAGE);
	}

	if (this->zombie_vision->intersect(this->player->rect())) {
		this->current_ai_state = SEARCH_PLAYER;
	}
	else{
		this->current_ai_state = SEARCH_BEACON;
	}

	this->zombie_vision->x = m_rect.x + m_rect.w/2 - this->zombie_vision->w/2;
	this->zombie_vision->y = m_rect.y -  m_rect.h/2 - this->zombie_vision->h/2;

	m_move = {0, 0};
}

void Zombie::update(f64 dt) {
	if(this->current_ai_state == ATTACK_BEACON) {
		this->beacon->add_power(-ZOMBIE_ATTACK_RATE * dt);
	}

	if(this->current_ai_state == SEARCH_PLAYER || this->current_ai_state == ATTACK_PLAYER) {
		this->animator->switch_frame(ZOMBIE_ATTACK);
	}

	this->update_ai(this->beacon);
	this->handle_collision(dt);

}

void Zombie::on_damage() {
	this->animator->switch_frame(ZOMBIE_ATTACK);
	if(m_health <= 0.0f) {
		m_entities.erase(
				std::remove(m_entities.begin(), m_entities.end(), this),
				m_entities.end()
		);

		Gem* gem = new Gem(m_entities, GEM_EXP);
		gem->set_pos({m_rect.x, m_rect.y});
		m_entities.push_back(gem);

		// This is dangerous but slokie used to so fuck it we ball
		delete this;

	}
}

void Zombie::render(const SpriteManager& sprt_mgr, std::vector<Quad>& quads, std::vector<Light>& lights) {
	this->update(this->fc->dt());
	if(is_stunned()) {
		m_overlay.a = 1.0f;
	} else{
		m_overlay.a = 0.0f;
	}

	quads.push_back( Quad {
			.pos = {m_rect.x, m_rect.y, 0},
			.size = {m_rect.w, m_rect.h},
			.rot = glm::mat4(1),
			.texture = &sprt_mgr.get_spritesheet_texture(ZOMBIE),
			.uv = this->animator->get_frame(sprt_mgr),
			.color = {1, 1, 1, 1},
			.overlay = m_overlay,
		});
}
