#ifndef _Display_h_
#define _Display_h_

#include "stdafx.h"

#include <imgui.h>

#include "GL/imgui_impl_glfw.h"
#include "GL/imgui_impl_opengl3.h"

#include "GL/Camera.h"
#include "GL/Mesh.h"
#include "GL/Texture.h"

#include "Generator/Generator.h"
#include "Generator/Terrain.h"

struct DisplaySettings {
	bool elevation = true;
	bool moisture = false;
	bool heatmap = false;
	bool biome = false;
};

struct DisplayObjects {
	std::shared_ptr<Camera> camera;

	std::shared_ptr<Program> material;
	std::shared_ptr<Mesh> quad;

	Texture elevation;
	Texture atmosphere;
	Texture heatmap;
	Texture biome;
};

void glfw_init();
void object_init(Generator &gen, DisplayObjects &objs);

void update_images(Generator &gen, DisplayObjects &objs);

void object_cleanup(DisplayObjects &objs);

void resize(DisplayObjects &objs, const int &width, const int &height);

void render(GLFWwindow *window, Generator &gen, DisplaySettings &ds, DisplayObjects &objs);
void render_ui(GLFWwindow *window, Generator &gen, TerrainSettings &ts, DisplaySettings &ds, DisplayObjects &objs);

#endif
