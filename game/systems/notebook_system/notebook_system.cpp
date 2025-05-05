#include "notebook_system.h"
#include "game_state.h"
#include "config.h"

extern GameState gs;

// TODO: Add support for long text wraps
// TODO: Make fixed amount of pages

NotebookSystem notebook_system_create() {
	return (NotebookSystem) {
		.current_page = 0,
		.open = false,
	};
}

void notebook_system_update(NotebookSystem* ns) {
	if (!ns->open) return;

	rp_push_quad(
		gs.quad_rp,
		glm::vec3(0),
		glm::vec2(WIN_WIDTH, WIN_HEIGHT),
		glm::vec4(0, 0, 0, 0.4),
		gs.quad_rp->white_texture.id,
		glm::vec4(0, 0, 1, 1)
	);

	// Rendering notebook
	glm::vec3 notebook_pos = {
		WIN_WIDTH / 2 - NOTEBOOK_WIDTH / 2,
		WIN_HEIGHT / 2 - NOTEBOOK_HEIGHT / 2,
		0
	};
	rp_push_quad(
		gs.quad_rp,
		notebook_pos,
		glm::vec2(NOTEBOOK_WIDTH, NOTEBOOK_HEIGHT),
		glm::vec4(0.97, 0.89, 0.74, 1),
		gs.quad_rp->white_texture.id,
		glm::vec4(0, 0, 1, 1)
	);

	// Rendering page number
	std::string page_no = std::to_string(ns->current_page + 1);
	glm::vec2 size = font_calc_size(gs.font_regular, page_no);
	glm::vec3 page_no_pos = {
		notebook_pos.x + NOTEBOOK_WIDTH / 2 - size.x / 2,
		notebook_pos.y + NOTEBOOK_HEIGHT - size.y,
		0
	};
	rp_push_text(
		gs.quad_rp,
		gs.font_regular,
		page_no,
		page_no_pos,
		glm::vec4(0, 0, 0, 1)
	);

	// If empty just return
	if (ns->pages.size() == 0) return;

	panic(ns->current_page < ns->pages.size(), "Page number is greater than number of pages. Pg no: %d, No of pages: %d", ns->current_page, ns->pages.size());

	Page page = ns->pages[ns->current_page];

	// Rendering texts
	glm::vec3 line_pos = {
		notebook_pos.x + NOTEBOOK_PAD_X,
		notebook_pos.y + NOTEBOOK_PAD_Y,
		0
	};
	for (auto& line : page.lines) {
		rp_push_text(
			gs.quad_rp,
			gs.font_regular,
			line,
			line_pos,
			glm::vec4(0,0,0,1)
		);
		line_pos.y += font_size(gs.font_regular).y;
	}
}

void notebook_system_handle_event(NotebookSystem* ns, SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				ns->open = false;
				break;

			case SDLK_LEFT:
				notebook_system_go_backward(ns);
				break;
			case SDLK_RIGHT:
				notebook_system_go_forward(ns);
				break;
		}
	}
}

void notebook_system_append_text(NotebookSystem* ns, const std::string& text) {
	// If no page exists just add the first page
	if (ns->pages.size() == 0) {
		Page page = {
			.no = ns->current_page,
		};
		ns->pages.push_back(page);
	}

	Page* page = &ns->pages[ns->current_page];
	glm::vec2 size = font_size(gs.font_regular);

	// Calculating max no of lines in a page
	// (total height - page number size) / height of each character 
	i32 max_lines = (NOTEBOOK_HEIGHT - size.y - 2*NOTEBOOK_PAD_Y) / size.y;

	// If the line is overflown add a new page
	if (page->lines.size() >= max_lines) {
		ns->current_page++;

		Page new_page = {
			.no = ns->current_page,
		};
		ns->pages.push_back(new_page);
	}

	// Appending the text to the desired page
	Page* final_page = &ns->pages[ns->current_page];
	final_page->lines.push_back(text);
}

void notebook_system_go_forward(NotebookSystem* ns) {
	if (ns->current_page < (i32) (ns->pages.size() - 1))
		ns->current_page++;
}

void notebook_system_go_backward(NotebookSystem* ns) {
	if (ns->current_page > 0)
		ns->current_page--;
}
