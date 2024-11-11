#ifndef PROTOTYPE_HPP
# define PROTOTYPE_HPP

#include <engine.hpp>
engine_t init_engine(void);

void update_light(std::vector<light_t> &lights, Shader light_shader);
void create_light(std::vector<light_t> &lights, Vector3 pos, int type, int intensity, Shader light_shader);
void render(level_t level, RenderTexture2D fbo, Camera3D camera, Shader posprocess_shader, Shader light_shader);

#endif