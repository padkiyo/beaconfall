#pragma once

#include "core.h"

enum class MsgSide {
	LEFT,
	RIGHT,
};

struct Message {
	i32 by;
	std::string text;
};

struct RenderableMessage {
	f32 y;
	MsgSide side;
	Message msg;
};

struct Dialog {
	i32 id;
	std::vector<Message> messages;
};

struct DialogSystem {
	b32 is_dialog_running;
	i32 current_dialog;
	i32 message_index;
	std::unordered_map<i32, Dialog> dialogs;

	// renderings
	std::unordered_map<i32, MsgSide> message_side;
	std::vector<RenderableMessage> interpolated_msgs;
	std::vector<RenderableMessage> to_interpolate;
};

DialogSystem* dialog_system_create();
void dialog_system_init_dialogs(DialogSystem* ds);
void dialog_system_start_dialog(DialogSystem* ds, i32 dialog_id);
void dialog_system_update(DialogSystem* ds);
void dialog_system_handle_event(DialogSystem* ds, SDL_Event event);
void dialog_system_move_forward(DialogSystem* ds);

