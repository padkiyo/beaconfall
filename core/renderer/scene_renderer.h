#pragma once

#include "common.h"
#include "renderer.h"
#include "scene/scene.h"
#include "camera/camera.h"

class SceneRenderer {
public:
	SceneRenderer(Renderer* m_renderer, const glm::vec2& res);
	~SceneRenderer();

	void render_scene(const Scene& scene, Camera& camera, const glm::vec2& res);

private:
	Renderer* m_renderer;
	Shader* m_color_shader;
	Shader* m_light_shader;
	Camera* m_camera;
};
