#ifndef ENGINE_HPP
# define ENGINE_HPP

#include <vector>
#define GLSL_VERSION 330
#include <raylib.h>
#include <glad.h>
#include <rlgl.h>
#include <raymath.h>
#include <rcamera.h>

#define MAX_LIGHTS  4
#define RL_READ_FRAMEBUFFER                     0x8CA8      // GL_READ_FRAMEBUFFER
#define RL_DRAW_FRAMEBUFFER                     0x8CA9      // GL_DRAW_FRAMEBUFFER

typedef enum {
    LIGHT_DIRECTIONAL = 0,
    LIGHT_POINT
} LightType;

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
	int type;
	bool enabled;
	Vector3 position;
	Vector3 target;
	Color color;
	float attenuation;

	// Shader locations
	int enabledLoc;
	int typeLoc;
	int positionLoc;
	int targetLoc;
	int colorLoc;
	int attenuationLoc;
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
	unsigned int zTexture;
}	gbuffer_t;

typedef enum {
	DEFERRED_POSITION,
	DEFERRED_NORMAL,
	DEFERRED_ALBEDO,
	DEFERRED_SHADING,
	DEFERRED_Z,
} deferred_mode;
 
typedef enum {
	GLOBAL_LIGHT,
	DIRECTIONNAL_LIGHT,
}	light_type;

typedef struct s_engine {
	Shader posprocess;
	Shader gbuffer_shader;
	Shader deffered_shader;
	gbuffer_t gbuffer;
	Camera3D camera;
	RenderTexture2D fbo;
	deferred_mode mode;
	Model cube;
	Model Sphere;
	light_t lights[MAX_LIGHTS];
}	engine_t;

typedef struct sv_player_s {
	Vector3 pos;

}	sv_player_t;

#endif