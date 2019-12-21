#include "stdafx.h"

#include "Display.h"

#include "Generator/Generator.h"

void glfw_error_callback(int error, const char *message) {
	printf("Encountered an error [%i]:%s\n", error, message);
	exit(EXIT_FAILURE);
}

void glfw_init() {
	glfwSetErrorCallback(glfw_error_callback);

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void object_init(Generator &gen, DisplayObjects &objs) {
	objs.camera = std::make_shared<Camera>(5.f, 800.f / 600.f, 0.01f, 15.f);
	objs.camera->setPosition({ 0, 0, -2 }, { 0, 0, 0 });

	objs.material = std::make_shared<Program>("textured_quad.vert", "textured_quad.frag");

	std::shared_ptr<Geometry> geom = std::make_shared<Geometry>();
	geom->set_vertices({ {1, 1, 0 }, {-1, 1, 0}, {1, -1, 0}, {-1, -1, 0} });
	geom->set_indices({ 0, 2, 1, 3, 1, 2 });
	geom->set_normals({ {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1} });
	geom->set_texture_coordinates({ {1, 1}, {0, 1}, {1, 0}, {0, 0} });

	objs.quad = std::make_shared<Mesh>(geom, objs.material);
	objs.quad->create();

	objs.elevation.create(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_FLOAT);
	objs.atmosphere.create(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_FLOAT);
	objs.heatmap.create(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_FLOAT, false);
	objs.biome.create(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, false);

	objs.material->bind_parameter("Elevation", objs.elevation);
	objs.material->bind_parameter("Atmosphere", objs.atmosphere);
	objs.material->bind_parameter("Heatmap", objs.heatmap);
	objs.material->bind_parameter("Biome", objs.biome);
}

void update_images(Generator &gen, DisplayObjects &objs) {
	const int width = gen.world.width;
	const int height = gen.world.height;

	objs.elevation.upload(&gen.world.elevation[0], width, height);
	objs.atmosphere.upload(&gen.world.atmosphere[0], width, height);
	objs.heatmap.upload(&gen.world.temperature[0], width, height);

	objs.biome.upload(&gen.world.biome[0], width, height);
}

void object_cleanup(DisplayObjects &objs) {
	objs.quad->destroy();
}

void resize(DisplayObjects &objs, const int &width, const int &height) {
	objs.camera->resize(width, height);
}

void render(GLFWwindow *window, Generator &gen, DisplaySettings &ds, DisplayObjects &objs) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	objs.camera->bind();

	objs.material->bind_parameter("display_elevation", ds.elevation);
	objs.material->bind_parameter("display_atmosphere", ds.moisture);
	objs.material->bind_parameter("display_heatmap", ds.heatmap);
	objs.material->bind_parameter("display_biome", ds.biome);

	objs.quad->render(objs.camera);

	objs.camera->unbind();
}

void render_ui(GLFWwindow *window, Generator &gen, TerrainSettings &ts, DisplaySettings &ds, DisplayObjects &objs) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();
	ImGui::Begin("Terrain Size");
	ImGui::SliderInt("World Width", &ts.map.world_width, 64, 1024);
	ImGui::SliderInt("World Height", &ts.map.world_height, 64, 1024);
	ImGui::SliderInt("Region Width", &ts.map.region_width, 1, 64);
	ImGui::SliderInt("Region Height", &ts.map.region_height, 1, 64);
	ImGui::SliderInt("Local Width", &ts.map.local_width, 1, 32);
	ImGui::SliderInt("Local Height", &ts.map.local_height, 1, 32);
	ImGui::SliderInt("Seed", &ts.map.seed, 0, 5000);
	ImGui::End();

	ImGui::Begin("Elevation Settings");
	ImGui::SliderFloat("Offset X", &ts.elevation.offset_x, 0, 10);
	ImGui::SliderFloat("Offset Y", &ts.elevation.offset_y, 0, 10);
	ImGui::SliderFloat("Amplitude Multiplier", &ts.elevation.amplitude_multiplier, 0, 10);
	ImGui::SliderInt("Fractal Sum Layers", &ts.elevation.fractalsum_layers, 1, 10);
	ImGui::SliderFloat("Frequency", &ts.elevation.frequency, 0, 10);
	ImGui::SliderFloat("Frequency X", &ts.elevation.frequency_x, 0, 10);
	ImGui::SliderFloat("Frequency Y", &ts.elevation.frequency_y, 0, 10);
	ImGui::SliderFloat("Frequency Multiplier", &ts.elevation.frequency_multiplier, 0, 10);
	ImGui::SliderInt("Elevation Offset", &ts.elevation.offset, 0, 10);
	ImGui::SliderInt("Elevation Range", &ts.elevation.range, 0, 255);
	ImGui::SliderInt("Ocean Level", &ts.elevation.terrain_ocean_bin, 0, 255);

	if (ImGui::Button("Generate")) {
		print_settings(ts);
		gen.generate(ts);

		update_images(gen, objs);
	}

	ImGui::End();

	ImGui::Begin("Moisture Settings");
	ImGui::SliderFloat("Offset X", &ts.moisture.offset_x, 0, 10);
	ImGui::SliderFloat("Offset Y", &ts.moisture.offset_y, 0, 10);
	ImGui::SliderFloat("Amplitude Multiplier", &ts.moisture.amplitude_multiplier, 0, 10);
	ImGui::SliderInt("Fractal Sum Layers", &ts.moisture.fractalsum_layers, 1, 10);
	ImGui::SliderFloat("Frequency", &ts.moisture.frequency, 0, 10);
	ImGui::SliderFloat("Frequency X", &ts.moisture.frequency_x, 0, 10);
	ImGui::SliderFloat("Frequency Y", &ts.moisture.frequency_y, 0, 10);
	ImGui::SliderFloat("Frequency Multiplier", &ts.moisture.frequency_multiplier, 0, 10);

	if (ImGui::Button("Generate Moisture")) {
		gen.generate(ts);

		update_images(gen, objs);
	}

	ImGui::End();

	ImGui::Begin("Temperature");
	ImGui::SliderInt("Polar", &ts.temperature.polar_shift, -50, 50);
	ImGui::SliderFloat("Temperature", &ts.temperature.temperature, 0, 50);
	ImGui::SliderFloat("Range", &ts.temperature.range, 0.001, 10);
	ImGui::SliderFloat("Scaling", &ts.temperature.scaling, 0.001, 10.0);
	ImGui::SliderFloat("Factor", &ts.temperature.factor, 0.001, 10.0);
	ImGui::End();

	ImGui::Begin("Save World");

	if (ImGui::Button("Save")) {
		save_world(gen.world, getCurrentDirectory() + "\\world");
	}

	ImGui::End();

	ImGui::Begin("Display");
	ImGui::Checkbox("Elevation", &ds.elevation);
	ImGui::Checkbox("Moisture", &ds.moisture);
	ImGui::Checkbox("Heatmap", &ds.heatmap);
	ImGui::Checkbox("Biome", &ds.biome);
	ImGui::End();

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

