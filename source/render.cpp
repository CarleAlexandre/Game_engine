#include <raylib.h>
#include <engine.hpp>

void create_light(std::vector<light_t> lights, point_t pos, int type, int intensity, Shader light_shader) {
	light_t light = {0};

	light.intensity = intensity;
	light.pos = pos;
	light.type = type;

	light.pos_loc = GetShaderLocation(light_shader, "light[%i].pos");
	light.type_loc = GetShaderLocation(light_shader, "light[%i].type");
	light.intensity_loc = GetShaderLocation(light_shader, "light[%i].intensity");

	lights.push_back(light);
	update_light(lights, light_shader);
}

void update_light(std::vector<light_t> lights, Shader light_shader) {
	for (auto light: lights) {
		SetShaderValue(light_shader, light.pos_loc, &light.pos, SHADER_UNIFORM_VEC2);
		SetShaderValue(light_shader, light.type_loc, &light.type, SHADER_UNIFORM_INT);
		SetShaderValue(light_shader, light.intensity_loc, &light.intensity, SHADER_UNIFORM_INT);
	}
}

void entity_culling() {

}

void object_culling() {

}

void walls_culling(scene_t &scene, Camera2D camera) {
	
}

void render(map_t map, RenderTexture2D fbo, Camera2D camera, Shader posprocess_shader, Shader light_shader) {
	scene_t scene;
	walls_culling(scene, camera);

	BeginTextureMode(fbo);
		ClearBackground(BLACK);
		BeginMode2D(camera);
			for (auto span : scene.walls) {
				DrawLine(span->p1.x, span->p2.x, span->p1.y, span->p2.y, WHITE);
			}
			for (auto span : scene.objects) {
				DrawCircle(span->pos.x, span->pos.y, span->rad, GREEN);
			}
			for (auto span : scene.entities) {
				DrawCircle(span->pos.x, span->pos.y, span->size, RED);
			}
			for (auto span : scene.players) {
				DrawCircle(span->pos.x, span->pos.y, 5, BLUE);
			}
		EndMode2D();
	EndTextureMode();

	BeginDrawing();
		ClearBackground(BLACK);
		BeginShaderMode(posprocess_shader);
			DrawTextureRec(fbo.texture, {0, 0, (float)GetScreenWidth(), (float)-GetScreenHeight()}, {0, 0}, WHITE);
		EndShaderMode();
	EndDrawing();
}


