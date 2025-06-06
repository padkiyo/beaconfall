#pragma once

#include "core.h"

struct Page {
	i32 no;
	std::vector<std::string> lines;
};

struct NotebookSystem {
	std::vector<Page> pages;
	i32 current_page;
	b32 open;
};

NotebookSystem* notebook_system_create();
void notebook_system_destroy(NotebookSystem* ns);
void notebook_system_update(NotebookSystem* ns);
void notebook_system_handle_event(NotebookSystem* ns, SDL_Event event);
void notebook_system_append_text(NotebookSystem* ns, const std::string& text);
void notebook_system_go_forward(NotebookSystem* ns);
void notebook_system_go_backward(NotebookSystem* ns);
