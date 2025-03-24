#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <stdlib.h>

typedef struct s_gbuffer{
	//framebuffer
	unsigned int	framebuffer;
	unsigned int	depthRenderbuffer;
	//texture	
	unsigned int	positionTexture;
	unsigned int	normalTexture;
	unsigned int	albedoSpecTexture;
	unsigned int	zTexture;
	//format
	unsigned int	width;
	unsigned int	height;
}	gbuffer_t;

gbuffer_t	haven_gbuffer_init(int width, int height) {
	gbuffer_t buffer = {0};
	buffer.height = height;
	buffer.width = width;

	buffer.framebuffer = rlLoadFramebuffer();
	if (!buffer.framebuffer) {
		TraceLog(LOG_WARNING, "failed to create framebuffer");
		exit(1);
	}

	rlEnableFramebuffer(buffer.framebuffer);
	buffer.positionTexture = rlLoadTexture(NULL, width,height, RL_PIXELFORMAT_UNCOMPRESSED_R16G16B16, 1);
	buffer.normalTexture = rlLoadTexture(NULL, width,height, RL_PIXELFORMAT_UNCOMPRESSED_R16G16B16, 1);
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

	return (buffer);
}

void	haven_deferred_set_loc(Shader shader) {
	rlEnableShader(shader.id);
		int tex_unit_posistion = 0;
		int tex_unit_normal = 1;
		int tex_unit_albedospec = 2;
		int tex_unit_z = 3;
		SetShaderValue(shader, rlGetLocationUniform(shader.id, "gPosition"), &tex_unit_posistion, RL_SHADER_UNIFORM_SAMPLER2D);
		SetShaderValue(shader, rlGetLocationUniform(shader.id, "gNormal"), &tex_unit_normal, RL_SHADER_UNIFORM_SAMPLER2D);
		SetShaderValue(shader, rlGetLocationUniform(shader.id, "gAlbedoSpec"), &tex_unit_albedospec, RL_SHADER_UNIFORM_SAMPLER2D);
		SetShaderValue(shader, rlGetLocationUniform(shader.id, "gZ"), &tex_unit_z, RL_SHADER_UNIFORM_SAMPLER2D);
	rlDisableShader();
}

void	haven_gbuffer_start_draw(const gbuffer_t gbuffer, const Camera3D camera, const Shader gbuffershader) {
	rlEnableFramebuffer(gbuffer.framebuffer);
	rlClearColor(0, 0, 0, 0);
	rlClearScreenBuffers();
		
	rlDisableColorBlend();
	BeginMode3D(camera);
	rlEnableShader(gbuffershader.id);		
}

void	haven_gbuffer_end_draw() {
	rlDisableShader();
	EndMode3D();

	rlEnableColorBlend();
	rlDisableFramebuffer();
	rlClearScreenBuffers();
}

void	haven_gbuffer_rendering(const gbuffer_t gbuffer, const Camera3D camera, const Shader deferredshader) {
	BeginMode3D(camera); {
		rlDisableColorBlend();
                rlEnableShader(deferredshader.id); {

			// Bind our g-buffer textures
			// We are binding them to locations that we earlier set in sampler2D uniforms `gPosition`, `gNormal`,
			// `gAlbedoSpec` and gZ
			rlActiveTextureSlot(0);
			rlEnableTexture(gbuffer.positionTexture);
			rlActiveTextureSlot(1);
			rlEnableTexture(gbuffer.normalTexture);
			rlActiveTextureSlot(2);
			rlEnableTexture(gbuffer.albedoSpecTexture);
			rlActiveTextureSlot(3);
			rlEnableTexture(gbuffer.zTexture);
			
			// Finally, we draw a fullscreen quad to our default framebuffer
			// This will now be shaded using our deferred shader
			rlLoadDrawQuad();
		}
		rlDisableShader();
		rlEnableColorBlend();
	} EndMode3D();
		
	// As a last step, we now copy over the depth buffer from our g-buffer to the default framebuffer.
	rlBindFramebuffer(RL_READ_FRAMEBUFFER, gbuffer.framebuffer);
	rlBindFramebuffer(RL_DRAW_FRAMEBUFFER, 0);
	rlBlitFramebuffer(0, 0, gbuffer.width, gbuffer.height, 0, 0, gbuffer.width, gbuffer.height, 0x00000100);//0x00000100 -> GL_DEPTH_BUFFER_BIT
	rlDisableFramebuffer();
}

void	haven_gbuffer_texture_render(const unsigned int texture_id, const unsigned int width, const unsigned int height) {
	DrawTextureRec((Texture2D){
		.id = texture_id,
		.width = width,
		.height = -height,
	}, (Rectangle) {0, 0, width, -height}, Vector2Zero(), RAYWHITE);
}