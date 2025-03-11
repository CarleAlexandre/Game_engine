#include "gl_impl.h"

const char default_vertex_shader[] = "\
#version 330 core\
layout (location = 0) in vec3 aPos;\
layout (location = 1) in vec2 aTexCoord;\
\
out vec2 TexCoord;\
\
uniform mat4 model;\
uniform mat4 view;\
uniform mat4 projection;\
\
void main()\
{\
	gl_Position = projection * view * model * vec4(aPos, 1.0f);\
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);\
}\
";

const char default_fragment_shader[] = "#version 460 core\n"\
"out vec4 FragColor;\n"\
"in vec2 TexCoord;\n"\
"void main() {\n"\
"	FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"\
"}\n";

GLuint	haven_opengl_shader_program_createf(const char *vs_file, const char *gs_file, const char *fs_file) {
	unsigned int shader_id = 0, vs = 0, gs = 0, fs = 0;
	int success = {0};
	char infoLog[512] = {0};

	shader_id = glCreateProgram();
	if (shader_id = 0) {
		haven_cmd_log("ERROR", "couldn't create shader program.", __LINE__);
		abort();
	}
	
	char *vs_txt = haven_file_read(vs_file);
	vs = glCreateShader(GL_VERTEX_SHADER);
	if (vs_txt) {
		glShaderSource(vs, 1, (const char **)&vs_txt, NULL);
		free(vs_txt);
	} else {
		glShaderSource(vs, 1, (const char **)&default_vertex_shader, NULL);
	}
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vs, 512, NULL, infoLog);
		haven_cmd_log("ERROR", infoLog, __LINE__);
	}
	glAttachShader(shader_id, vs);

	char *gs_txt = haven_file_read(gs_file);
	if (!gs_txt) goto fragment_step;
	gs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(gs, 1, (const char **)&gs_txt, NULL);
	free(gs_txt);
	glCompileShader(gs);
	glGetShaderiv(gs, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(gs, 512, NULL, infoLog);
		haven_cmd_log("ERROR", infoLog, __LINE__);
	}
	glAttachShader(shader_id, gs);
	
	fragment_step:
	char *fs_txt = haven_file_read(fs_file);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	if (fs_txt) {
		glShaderSource(fs, 1, (const char **)&fs_txt, NULL);
		free(fs_txt);
	} else {
		glShaderSource(fs, 1, (const char **)&default_fragment_shader, NULL);
	}
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vs, 512, NULL, infoLog);
		haven_cmd_log("ERROR", infoLog, __LINE__);
	}
	glAttachShader(shader_id, fs);

	glLinkProgram(shader_id);
	glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shader_id, 512, NULL, infoLog);
		haven_cmd_log("ERROR", infoLog, __LINE__);
	}

	haven_cmd_log("INFO", "Shader Loaded Succesfully", __LINE__);
	if (vs) glDeleteShader(vs);
	if (gs) glDeleteShader(gs);
	if (fs) glDeleteShader(fs);
	return (shader_id);
}

GLuint	haven_opengl_shader_program_create(const char *vertex, const char *geometry, const char *fragment) {
	unsigned int shader_id, vs, gs, fs;
	int success;
	char infoLog[512];

	shader_id = glCreateProgram();
	if (shader_id = 0) {
		haven_cmd_log("ERROR", "couldn't create shader program.", __LINE__);
		abort();
	}
	
	vs = glCreateShader(GL_VERTEX_SHADER);
	if (!vertex) {
		vertex = default_vertex_shader;
	}
	glShaderSource(vs, 1, (const char **)&vertex, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vs, 512, NULL, infoLog);
		haven_cmd_log("ERROR", infoLog, __LINE__);
	}
	glAttachShader(shader_id, vs);

	gs = glCreateShader(GL_VERTEX_SHADER);
	if (!geometry) goto fragment_step;
	glShaderSource(gs, 1, (const char **)&geometry, NULL);
	glCompileShader(gs);
	glGetShaderiv(gs, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(gs, 512, NULL, infoLog);
		haven_cmd_log("ERROR", infoLog, __LINE__);
	}
	glAttachShader(shader_id, gs);
	
	fragment_step:
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	if (!fragment) {
		fragment = default_fragment_shader;
	}
	glShaderSource(fs, 1, (const char **)&fragment, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vs, 512, NULL, infoLog);
		haven_cmd_log("ERROR", infoLog, __LINE__);
	}
	glAttachShader(shader_id, fs);

	glLinkProgram(shader_id);
	glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shader_id, 512, NULL, infoLog);
		haven_cmd_log("ERROR", infoLog, __LINE__);
	}

	haven_cmd_log("INFO", "Shader Loaded Succesfully", __LINE__);
	if (vs) glDeleteShader(vs);
	if (gs) glDeleteShader(gs);
	if (fs) glDeleteShader(fs);
	return (shader_id);
}

void	haven_opengl_shader_close(GLint shader_id){
	glDeleteProgram(shader_id);
}

void	haven_opengl_render_begin_frame() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void	haven_opengl_render_end_frame(GLFWwindow *window) {
	glfwSwapBuffers(window);
	glfwPollEvents();
	haven_time_update();
}

GLFWwindow *haven_opengl_init(const char *title, const short height, const short width) {
	GLFWwindow *window = 0x00;
	haven_time_init();
	
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window) {
		haven_cmd_log("ERROR", "couldn't Create Window", __LINE__);
		return (window);
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		haven_cmd_log("ERROR", "couldn't initialize GLAD", __LINE__);
		return (window);
	}

	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	return (window);
}

void	haven_opengl_close(GLFWwindow *window) {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void	haven_opengl_shader_begin(GLuint shader_id) {
	glUseProgram(shader_id);
	//upload uniform
}

void	haven_opengl_shader_end() {
	glUseProgram(0);
}
