#ifndef PROTOTYPE_HPP
# define PROTOTYPE_HPP

#include <engine.hpp>
engine_t init_engine(void);
void close_engine(engine_t &engine);

void update_light(std::vector<light_t> &lights, Shader light_shader);
void create_light(std::vector<light_t> &lights, Vector3 pos, int type, int intensity, Shader light_shader);
void render(level_t level, engine_t &engine, void (*render_ui)(void));

void update_input(engine_t &engine);

#endif