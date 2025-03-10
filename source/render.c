#include <prototype.h>

const float	quad_vertices[] = {
	0.0f, 0.0f, 0.0f, //bot_left
	0.5f, 0.0f, 0.0f,//bot_right
	0.0f, 0.5f, 0.0f,//top_left
	0.5f, 0.5f, 0.0f //top_right
};

const uint32_t	quad_indices[] = {0, 1, 2, 3};

void	ExtractFrustumFromMatrix(Matrix mat, Frustum *frustum) {
	// Left plane
	frustum->planes[0].normal.x = mat.m3 + mat.m0;
	frustum->planes[0].normal.y = mat.m7 + mat.m4;
	frustum->planes[0].normal.z = mat.m11 + mat.m8;
	frustum->planes[0].distance = mat.m15 + mat.m12;
    
	// Right plane
	frustum->planes[1].normal.x = mat.m3 - mat.m0;
	frustum->planes[1].normal.y = mat.m7 - mat.m4;
	frustum->planes[1].normal.z = mat.m11 - mat.m8;
	frustum->planes[1].distance = mat.m15 - mat.m12;
    
	// Bottom plane
	frustum->planes[2].normal.x = mat.m3 + mat.m1;
	frustum->planes[2].normal.y = mat.m7 + mat.m5;
	frustum->planes[2].normal.z = mat.m11 + mat.m9;
	frustum->planes[2].distance = mat.m15 + mat.m13;
    
	// Top plane
	frustum->planes[3].normal.x = mat.m3 - mat.m1;
	frustum->planes[3].normal.y = mat.m7 - mat.m5;
	frustum->planes[3].normal.z = mat.m11 - mat.m9;
	frustum->planes[3].distance = mat.m15 - mat.m13;
    
	// Near plane
	frustum->planes[4].normal.x = mat.m3 + mat.m2;
	frustum->planes[4].normal.y = mat.m7 + mat.m6;
	frustum->planes[4].normal.z = mat.m11 + mat.m10;
	frustum->planes[4].distance = mat.m15 + mat.m14;
    
	// Far plane
	frustum->planes[5].normal.x = mat.m3 - mat.m2;
	frustum->planes[5].normal.y = mat.m7 - mat.m6;
	frustum->planes[5].normal.z = mat.m11 - mat.m10;
	frustum->planes[5].distance = mat.m15 - mat.m14;
    
	// Normalize all planes
	for (int i = 0; i < 6; i++) {
		float length = Vector3Length(frustum->planes[i].normal);
		frustum->planes[i].normal = Vector3Normalize(frustum->planes[i].normal);
		frustum->planes[i].distance /= length;
	}
}

bool	IsBoxInFrustum(BoundingBox box, Frustum frustum) {
	for (int i = 0; i < 6; i++) {
		Vector3 positive = box.min;
		
		if (frustum.planes[i].normal.x >= 0) positive.x = box.max.x;
		if (frustum.planes[i].normal.y >= 0) positive.y = box.max.y;
		if (frustum.planes[i].normal.z >= 0) positive.z = box.max.z;

		float distance = Vector3DotProduct(frustum.planes[i].normal, positive) + 
				frustum.planes[i].distance;
				
		if (distance < 0.01) return false;
	}
	return true;
}

world_mesh_t	assemble_world_mesh(chunk_t *rqueue[512], unsigned int rcount) {
	// face_data_t	faces[10000000] = {0};
	// unsigned int	face_count = 0;
	// Vector3		pos[512] = {0};
	// unsigned int	pos_count = 0;

	// for (int i = 0; i < rcount; i++) {
	// 	for (int k = 0; k < rqueue[i]->mesh->faces_count; k++) {
	// 		faces[face_count++] = rqueue[i]->mesh->faces[k];
	// 	}
	// 	pos[pos_count++] = rqueue[i]->pos;
	// }

	// world_mesh_t mesh = {0};

	// mesh.faces = malloc(sizeof(face_data_t) * face_count);
	// mesh.faces_count = face_count;
	// mesh.pos = malloc(sizeof(Vector3) * pos_count);
	// mesh.pos_count = pos_count;
}

world_render_t	gen_world_render(world_mesh_t *mesh) {
	world_render_t render;

	glGenVertexArrays(1, &render.vao);
	glBindVertexArray(render.vao);

	glGenBuffers(1, &render.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, render.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 *sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &render.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices, GL_STATIC_DRAW);


	glGenBuffers(1, &render.ibo);
	glBindBuffer(GL_ARRAY_BUFFER, render.ibo);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_DYNAMIC_DRAW);

	rlSetVertexAttribute(1, 1, RL_FLOAT, false, sizeof(face_data_t), offsetof(face_data_t, face_data));
	rlEnableVertexAttribute(1);
	rlSetVertexAttributeDivisor(1, 1);

	rlSetVertexAttribute(2, 1, RL_FLOAT, false, sizeof(face_data_t), offsetof(face_data_t, block_id));
	rlEnableVertexAttribute(2);
	rlSetVertexAttributeDivisor(2, 1);

	glBindVertexArray(0);

	//ssbo
	glCreateBuffers(1, &render.ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, render.ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 0, 0, GL_STREAM_COPY);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	return (render);
}

// void	update_world_render(uint32_t id, void *queue) {
// 	glBindBuffer(GL_ARRAY_BUFFER, id);
// 	glBufferSubData(GL_ARRAY_BUFFER, 0, 0, 0);
// }

void	update_chunk_render(chunk_mesh_t *mesh) {
	int total_size = 0;

	for (int i = 0; i < 6; i++) {
		total_size += mesh->faces_count[i];
	}

	face_data_t *buffer = calloc(total_size, sizeof(face_data_t));

	int prev = 0;

	for (int i = 0; i < 6; i++) {
		memcpy(&buffer[prev], mesh->faces[i], mesh->faces_count[i] * sizeof(face_data_t));
		prev += mesh->faces_count[i];
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh->ibo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, total_size * sizeof(face_data_t), buffer);

}

void	gen_chunk_render(chunk_mesh_t *mesh) {
	int total_size = 0;

	for (int i = 0; i < 6; i++) {
		total_size += mesh->faces_count[i];
	}

	face_data_t *buffer = calloc(total_size, sizeof(face_data_t));

	int prev = 0;

	for (int i = 0; i < 6; i++) {
		memcpy(&buffer[prev], mesh->faces[i], mesh->faces_count[i] * sizeof(face_data_t));
		prev += mesh->faces_count[i];
	}

	mesh->vao = rlLoadVertexArray();
	rlEnableVertexArray(mesh->vao);

	mesh->vbo = rlLoadVertexBuffer(quad_vertices, sizeof(quad_vertices), false);
	rlSetVertexAttribute(0, 3, RL_FLOAT, false, 3 *sizeof(float), 0);
	rlEnableVertexAttribute(0);

	mesh->ebo = rlLoadVertexBufferElement(quad_indices, sizeof(quad_indices), false);

	mesh->ibo = rlLoadVertexBuffer(buffer, total_size * sizeof(face_data_t), true);

	rlSetVertexAttribute(1, 1, RL_FLOAT, false, sizeof(face_data_t), offsetof(face_data_t, face_data));
	rlEnableVertexAttribute(1);
	rlSetVertexAttributeDivisor(1, 1);

	rlSetVertexAttribute(2, 1, RL_FLOAT, false, sizeof(face_data_t), offsetof(face_data_t, block_id));
	rlEnableVertexAttribute(2);
	rlSetVertexAttributeDivisor(2, 1);

	rlDisableVertexArray();
}

gbuffer_t	loadGbuffer(int width, int height, Shader deffered_shader) {
	gbuffer_t buffer = {0};

	buffer.framebuffer = rlLoadFramebuffer();
	if (!buffer.framebuffer) {
		TraceLog(LOG_WARNING, "failed to create framebuffer");
		exit(1);
	}
	rlEnableFramebuffer(buffer.framebuffer);
	buffer.positionTexture = rlLoadTexture(NULL, width,height, RL_PIXELFORMAT_UNCOMPRESSED_R32G32B32, 1);
	buffer.normalTexture = rlLoadTexture(NULL, width,height, RL_PIXELFORMAT_UNCOMPRESSED_R32G32B32, 1);
	buffer.albedoSpecTexture = rlLoadTexture(NULL, width,height, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
	buffer.zTexture = rlLoadTexture(NULL, width,height, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
	buffer.depthRenderbuffer = rlLoadTextureDepth(width, height, true);
	rlActiveDrawBuffers(4);

	rlFramebufferAttach(buffer.framebuffer, buffer.positionTexture, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
	rlFramebufferAttach(buffer.framebuffer, buffer.normalTexture, RL_ATTACHMENT_COLOR_CHANNEL1, RL_ATTACHMENT_TEXTURE2D, 0);
	rlFramebufferAttach(buffer.framebuffer, buffer.albedoSpecTexture, RL_ATTACHMENT_COLOR_CHANNEL2, RL_ATTACHMENT_TEXTURE2D, 0);
	rlFramebufferAttach(buffer.framebuffer, buffer.zTexture, RL_ATTACHMENT_COLOR_CHANNEL3, RL_ATTACHMENT_TEXTURE2D, 0);
	rlFramebufferAttach(buffer.framebuffer, buffer.depthRenderbuffer, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);

	if (!rlFramebufferComplete(buffer.framebuffer)) {
		TraceLog(LOG_WARNING, "Framebuffer is not complete");
		exit(1);
	}

	rlEnableShader(deffered_shader.id);
		rlSetUniformSampler(rlGetLocationUniform(deffered_shader.id, "gPosition"), 0);
		rlSetUniformSampler(rlGetLocationUniform(deffered_shader.id, "gNormal"), 1);
		rlSetUniformSampler(rlGetLocationUniform(deffered_shader.id, "gAlbedoSpec"), 2);
		rlSetUniformSampler(rlGetLocationUniform(deffered_shader.id, "gZ"), 3);
	rlDisableShader();

	rlEnableDepthTest();
	rlEnableBackfaceCulling();
	return (buffer);
}

void	render_voxel_work(Shader shader, Matrix transform, world_t *world) {
	rlEnableShader(shader.id );

	Matrix matModel = MatrixIdentity();
	Matrix matView = rlGetMatrixModelview();
	Matrix matModelView = MatrixIdentity();
	Matrix matProjection = rlGetMatrixProjection();

	if (shader.locs[SHADER_LOC_MATRIX_VIEW] != -1) rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_VIEW], matView);
	if (shader.locs[SHADER_LOC_MATRIX_PROJECTION] != -1) rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_PROJECTION], matProjection);
	
	matModel = MatrixMultiply(transform, rlGetMatrixTransform());

	if (shader.locs[SHADER_LOC_MATRIX_MODEL] != -1) rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_MODEL], matModel);

	matModelView = MatrixMultiply(matModel, matView);

	if (shader.locs[SHADER_LOC_MATRIX_NORMAL] != -1) rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_NORMAL], MatrixTranspose(MatrixInvert(matModel)));

	Matrix matModelViewProjection = MatrixIdentity();
	matModelViewProjection = MatrixMultiply(matModelView, matProjection);

	rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_MVP], matModelViewProjection);

	// glMultiDrawArrays(GL_TRIANGLE_STRIP, 0, 0, 0);
	for (int i = 0; i < world->rcount; i++) {
		chunk_t *chk = world->rqueue[i];
		float pos[3] = {chk->pos.x, chk->pos.y, chk->pos.z};
		
		SetShaderValue(shader, GetShaderLocation(shader, "chunk_pos"), &pos[0], RL_SHADER_UNIFORM_VEC3);

		rlEnableVertexArray(chk->mesh->vao);

		int total = 0;
		for (int i = 0; i < 6; i++) {
			total += chk->mesh->faces_count[i];
		}
		
		glDrawElementsInstanced(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0, total);

		rlDisableVertexArray();
	}

	rlDisableShader();

	rlSetMatrixModelview(matView);
	rlSetMatrixProjection(matProjection);
}

void	voxel_render(engine_t *engine, world_t *world) {
	float camera_pos[3] = {engine->camera.position.x, engine->camera.position.y, engine->camera.position.z};
	int screen_height = GetScreenHeight(), screen_width = GetScreenWidth();

	BeginDrawing();
	ClearBackground(BLACK);
	
	BeginMode3D(engine->camera);
		rlDisableBackfaceCulling();
		rlDisableDepthMask();
		DrawModel(engine->skybox, (Vector3){0, 0, 0}, 1.0f, WHITE);
		rlEnableBackfaceCulling();
		rlEnableDepthMask();	
	
		render_voxel_work(engine->shader[shader_voxel_solid], MatrixIdentity(), world);

		Vector3 pos = {0};
		bool ret = voxel_look_at(engine->camera, world, 5, &pos);
		if (ret) {
			DrawCubeWires(pos, 0.5, 0.5, 0.5, BLACK);
		}

		// if (engine->debug == true) {
		// 	DrawBoundingBox(engine->player.bound, RED);
		// 	for (int i = 0; i <= 8; i++) {
		// 		for (int k = 0; k <= 8; k++) {
		// 			DrawLine3D((Vector3){0, i * 32, k * 32}, (Vector3){256,i * 32, k * 32}, RED);
		// 			DrawLine3D((Vector3){i * 32, 0, k * 32}, (Vector3){i * 32, 256, k * 32}, GREEN);
		// 			DrawLine3D((Vector3){i * 32, k * 32, 0}, (Vector3){i * 32, k * 32, 256}, BLUE);
		// 		}
		// 	}
		// }
	EndMode3D();
	draw_ui(engine->player);
	Vector3 forward = GetCameraForward(&engine->camera);
	DrawText(TextFormat("x:%.1f y:%.1f z:%.1f\n", forward.x, forward.y, forward.z), 10, 200, 20, GREEN);
	DrawText(TextFormat("x:%.0f y:%.0f z:%.0f\n", engine->camera.position.x, engine->camera.position.y, engine->camera.position.z), 10, 300, 20, GREEN);
	EndDrawing();
}
