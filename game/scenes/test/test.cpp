#include "test.h"

void TestScene::on_enter() {
	log_info("Test scene is entered\n");
}

void TestScene::on_exit() {
	log_info("Test scene is exited\n");
}

void TestScene::on_update(f64 dt) {
}

void TestScene::on_event(const SDL_Event& event, f64 dt) {
}

void TestScene::on_imgui_render() {
}
