#ifndef HAVEN_RENDER_H
# define HAVEN_RENDER_H

#include <raylib.h>

typedef struct s_gbuffer{
	unsigned int	framebuffer;
	unsigned int	positionTexture;
	unsigned int	normalTexture;
	unsigned int	albedoSpecTexture;
	unsigned int	depthRenderbuffer;
	unsigned int	zTexture;
	unsigned int	width;
	unsigned int	height;
}	gbuffer_t;

/**
 * @brief 
 * 
 * @param texture_id 
 * @param width 
 * @param height 
 */
void	haven_gbuffer_texture_render(const unsigned int texture_id, const unsigned int width, const unsigned int height);

/**
 * @brief 
 * 
 * @param gbuffer 
 * @param camera 
 * @param deferredshader 
 */
void	haven_gbuffer_rendering(const gbuffer_t gbuffer, const Camera3D camera, const Shader deferredshader);

/**
 * @brief 
 * 
 */
void	haven_gbuffer_end_draw(void);

/**
 * @brief 
 * 
 * @param gbuffer 
 * @param camera 
 * @param gbuffershader 
 */
void	haven_gbuffer_start_draw(const gbuffer_t gbuffer, const Camera3D camera, const Shader gbuffershader);

/**
 * @brief 
 * 
 * @param width 
 * @param height 
 * @param shader_id 
 * @return gbuffer_t 
 */
gbuffer_t	haven_gbuffer_init(const int width, const int height, const unsigned int shader_id);

#endif