#include "dialog_system.h"
#include "dialogs.h"
#include "config.h"
#include "game_state.h"

extern GameState gs;

DialogSystem* dialog_system_create() {
	DialogSystem* ds = new DialogSystem;
	ds->is_dialog_running = false;
	ds->current_dialog = -1;
	ds->message_index = 0;
	return ds;
}

void dialog_system_init_dialogs(DialogSystem* ds) {
	ds->dialogs.insert({ test_dialog.id, test_dialog });
}

void dialog_system_start_dialog(DialogSystem* ds, i32 dialog_id) {
	panic(ds->dialogs.find(dialog_id) != ds->dialogs.end(), "Cannot find dialog with id: %d", dialog_id);
	if (ds->current_dialog == dialog_id) return;

	ds->is_dialog_running = true;
	ds->current_dialog = dialog_id;
	ds->message_index = -1;

	// Moving forward the dialog as it begins with -1 in start
	dialog_system_move_forward(ds);
}

void dialog_system_update(DialogSystem* ds) {
	if (!ds->is_dialog_running) return;

	// Rendering an overlay to hide focus from game
	rp_push_quad(
		gs.quad_rp,
		glm::vec3(0),
		glm::vec2(WIN_WIDTH, WIN_HEIGHT),
		glm::vec4(0, 0, 0, 0.4),
		gs.quad_rp->white_texture.id,
		glm::vec4(0, 0, 1, 1)
	);

	Dialog dialog = ds->dialogs[ds->current_dialog];
	
	// End of dialog
	if (ds->message_index >= dialog.messages.size()) {
		ds->is_dialog_running = false;
		ds->current_dialog = -1;
		return;
	}

#define BUBBLE_PADDING 10
#define Y_GAP 5

	// Render chat boxes
	glm::vec3 pos = {50, 500, 0};

	// Iterating from back to first
	for (auto it = ds->msg_to_render.rbegin(); it != ds->msg_to_render.rend(); ++it) {
		auto& rm = *it;
		MsgSide side = rm.side;
		Message msg = rm.msg;

		// Determining the X position of the text bubbles
		if (side == MsgSide::LEFT) {
			pos.x = WIN_WIDTH / 2 - 300;
		} else {
			pos.x = WIN_WIDTH / 2;
		}

		glm::vec2 size = font_calc_size(gs.font_regular, msg.text);

		// Going upwards
		pos.y -= size.y + BUBBLE_PADDING + Y_GAP;

		// Adding padding to the bubble
		glm::vec2 box_size = {
			size.x + BUBBLE_PADDING,
			size.y + BUBBLE_PADDING,
		};

		glm::vec3 box_pos = {
			pos.x - BUBBLE_PADDING / 2,
			pos.y - BUBBLE_PADDING / 2,
			pos.z
		};

		// Rendering bubble
		rp_push_quad(
			gs.quad_rp,
			box_pos,
			box_size,
			glm::vec4(1, 1, 1, 0.8),
			gs.quad_rp->white_texture.id,
			glm::vec4(0, 0, 1, 1)
		);

		// Rendering text
		rp_push_text(
			gs.quad_rp,
			gs.font_regular,
			msg.text,
			pos,
			glm::vec4(0, 0, 0, 1)
		);
	}
}

void dialog_system_handle_event(DialogSystem* ds, SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {

			// Skip the dialog
			case SDLK_ESCAPE:
				log_info("Exiting test dialog\n");
				ds->is_dialog_running = false;
				ds->current_dialog = -1;
				break;

			// Progress through the dialog
			case SDLK_SPACE:
				dialog_system_move_forward(ds);
				break;

		}
	}
}

void dialog_system_move_forward(DialogSystem* ds) {
	ds->message_index++;

	Dialog dialog = ds->dialogs[ds->current_dialog];

	// Just return if the msg index is out of bounds
	if (ds->message_index >= dialog.messages.size()) return;

	Message msg = dialog.messages[ds->message_index];

	// Setting on whose message should be in left and whose in right
	if (ds->message_side.find(msg.by) == ds->message_side.end()) {
		if (ds->message_side.size() == 0) {
			ds->message_side.insert({ msg.by, MsgSide::LEFT });
		} else {
			ds->message_side.insert({ msg.by, MsgSide::RIGHT });
		}
	}

	MsgSide side = ds->message_side[msg.by];

	RenderableMessage rm = {
		.side = side,
		.msg = msg,
	};

	// If the message to render is greater than 3
	// than remove the top one and add the new message at last
	if (ds->msg_to_render.size() >= 3) {

		// Removing the top one
		ds->msg_to_render.erase(ds->msg_to_render.begin());
	}

	// Pushing in the renderable messages
	ds->msg_to_render.push_back(rm);
}
