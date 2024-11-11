#include <engine.hpp>

engine_t init_engine(void) {
	engine_t engine;
	Camera3D camera;
	camera.fovy = 90;
	camera.position = {0, 10, 0};
	camera.projection = CAMERA_PERSPECTIVE;
	camera.target = {0, 0, 0};
	camera.up = {0, 1, 0};
	engine.camera = camera;
	return (engine);
}