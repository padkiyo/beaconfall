#pragma once

#include "dialog_system.h"

// IDs for all the dialogs
enum Dialogs {
	DIALOG_TEST,
};

// NOTE: These are temporary characters
enum Character {
	A,
	B,
};

/*
 * Test dialog
 */

static Dialog test_dialog = {
	.id = DIALOG_TEST,
	.messages = {
		{A, "Hey! How are you doing?"},
		{B, "Good. How about you?"},
		{A, "Fine."},
		{B, "Alright."},
		{A, "See ya."},
	}
};
