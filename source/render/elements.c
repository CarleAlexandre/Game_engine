#include "render_impl.h"

void	haven_voxel_render_elements(float pos[3], Shader shader) {	
	SetShaderValue(shader, GetShaderLocation(shader, "chunk_pos"), &pos[0], RL_SHADER_UNIFORM_VEC3);
	
	rlEnableVertexArray(0);
		// int quad_count;
		// glDrawElementsInstanced(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0, quad_count);
	rlDisableVertexArray();
}

void	haven_voxel_render_chunks(Shader shader, Matrix transform) {
	Matrix matModel = MatrixIdentity();
	Matrix matView = rlGetMatrixModelview();
	Matrix matProjection = rlGetMatrixProjection();
	rlEnableShader(shader.id ); {
		matModel = MatrixMultiply(transform, rlGetMatrixTransform());
		if (shader.locs[SHADER_LOC_MATRIX_MODEL] != -1) {
			rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_MODEL], matModel);
		}
		if (shader.locs[SHADER_LOC_MATRIX_VIEW] != -1) {
			rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_VIEW], matView);
		}
		if (shader.locs[SHADER_LOC_MATRIX_PROJECTION] != -1) {
			rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_PROJECTION], matProjection);
		}
		if (shader.locs[SHADER_LOC_MATRIX_NORMAL] != -1) {
			rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_NORMAL], MatrixTranspose(MatrixInvert(matModel)));
		}
		rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_MVP], MatrixMultiply(MatrixMultiply(matModel, matView), matProjection));
		for (int i = 0; i < 0; i++) {
			float pos[3];
			haven_voxel_render_elements(pos, shader);
		}
	} rlDisableShader();		
	rlSetMatrixModelview(matView);
	rlSetMatrixProjection(matProjection);
}
