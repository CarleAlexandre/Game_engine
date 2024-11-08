#include <raylib.h>
#include <engine.hpp>

void light_culling() {

}

void entity_culling() {

}

void object_culling() {

}

void walls_culling() {
	
}

void update_light() {

}

void render(scene_t scene, RenderTexture2D fbo, Camera2D camera, Shader posprocess_shader, Shader lightShader) {
	BeginTextureMode(fbo);
		ClearBackground(BLACK);
		BeginMode2D(camera);
			for (auto span : scene.walls);
			for (auto span : scene.objects);
			for (auto span : scene.entities);
			for (auto span : scene.players);
		EndMode2D();
	EndTextureMode();

	BeginDrawing();
		ClearBackground(BLACK);
		BeginShaderMode(posprocess_shader);
			DrawTextureRec(fbo.texture, {0, 0, (float)GetScreenWidth(), (float)-GetScreenHeight()}, {0, 0}, WHITE);
		EndShaderMode();
	EndDrawing();
}
