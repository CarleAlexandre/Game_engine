#ifndef ENGINE_HPP
# define ENGINE_HPP

#include <vector>
#define GLSL_VERSION 330
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>

typedef struct s_player {
	Vector3 pos;
}	player_t;

typedef struct s_object {
	Vector3 pos;
	float scale;
	int type;
}	object_t;

typedef struct s_item {

}	item_t;

typedef struct s_entity {
	Vector3 pos;
	int size;
	int layer ;
}	entity_t;

typedef struct s_wall {
	Vector3 p1;
	Vector3 p2;
	int layer;
}	wall_t;

typedef struct s_light {
	Vector3 pos;
	int type;
	int intensity;
	int pos_loc;
	int type_loc;
	int intensity_loc;
}	light_t;

typedef struct s_terrain {
	Model model;
	Vector3 pos;
	float scale;
}	terrain_t;

typedef struct s_level {
	terrain_t terrain;
	std::vector<object_t> objs;
	std::vector<light_t> lights;
}	level_t;

typedef struct s_gbuffer{
    unsigned int framebuffer;
    unsigned int positionTexture;
    unsigned int normalTexture;
    unsigned int albedoSpecTexture;
    unsigned int depthRenderbuffer;
}	gbuffer_t;

typedef enum {
   DEFERRED_POSITION,
   DEFERRED_NORMAL,
   DEFERRED_ALBEDO,
   DEFERRED_SHADING
} deferred_mode;

typedef enum {
	GLOBAL_LIGHT,
	DIRECTIONNAL_LIGHT,
}	light_type;

typedef struct s_engine {
	RenderTexture fbo;
	Shader posprocess;
	Shader light;
	Shader gbuffer_shader;
	Shader deffered_shader;
	gbuffer_t gbuffer;
	Camera3D camera;
	deferred_mode mode;
}	engine_t;

#endif