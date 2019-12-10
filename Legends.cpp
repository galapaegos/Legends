// Legends.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Generator.h"
//#include "TiffObject.h"

#include "tiffio.h"

#include <imgui.h>
#include "GL/imgui_impl_glfw.h"
#include "GL/imgui_impl_opengl3.h"

void warningHandler(const char *module, const char *fmt, va_list ap)
{
	char buffer[2048];
	memset(buffer, 0, 2048);

	_vsnprintf_s(buffer, sizeof(buffer), fmt, ap);

	printf("Warning: module - %s: %s\n", module, buffer);
}

void errorHandler(const char *module, const char *fmt, va_list ap)
{
	char buffer[2048];
	memset(buffer, 0, 2048);

	_vsnprintf_s(buffer, sizeof(buffer), fmt, ap);

	printf("Error: module - %s: %s\n", module, buffer);
}

void glfw_error_callback(int error, const char *message) {
	printf("Encountered an error [%i]:%s\n", error, message);
	exit(EXIT_FAILURE);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_callback(GLFWwindow *window, int button, int action, int mods) {
}

struct DisplaySettings {
	bool elevation = true;
	bool moisture = false;
	bool heatmap = false;
};

void init(Generator &gen) {

}

void render(GLFWwindow *window, Generator &gen, DisplaySettings &ds) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.f, float(width) / float(height), 0.01f, 20.f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	float texture_x = 1.f;
	float texture_y = 1.f;

	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(-texture_x, -texture_y, 0.f);
	glVertex3f(texture_x, -texture_y, 0.f);
	glVertex3f(texture_x, texture_y, 0.f);
	glVertex3f(-texture_x, texture_y, 0.f);
	glEnd();
}

void render_ui(GLFWwindow *window, Generator &gen, TerrainSettings &ts, DisplaySettings &ds) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();
	int width=256, height=256;
	int region_width=8, region_height=8;
	int local_width=8, local_height=8;
	int seed = 2019;
	ImGui::Begin("Terrain Size");
	ImGui::SliderInt("World Width", &width, 64, 1024);
	ImGui::SliderInt("World Height", &height, 64, 1024);
	ImGui::SliderInt("Region Width", &region_width, 1, 64);
	ImGui::SliderInt("Region Height", &region_height, 1, 64);
	ImGui::SliderInt("Local Width", &local_width, 1, 32);
	ImGui::SliderInt("Local Height", &local_height, 1, 32);
	ImGui::SliderInt("Seed", &seed, 0, 10000);
	ImGui::End();

	ImGui::Begin("Elevation Settings");
	ImGui::SliderFloat("Elevation Offset X", &ts.elevation.elevation_offset_x, 0, 10);
	ImGui::SliderFloat("Elevation Offset Y", &ts.elevation.elevation_offset_y, 0, 10);
	ImGui::SliderFloat("Amplitude Multiplier", &ts.elevation.elevation_amplitude_multiplier, 0, 10);
	ImGui::SliderInt("Fractal Sum Layers", &ts.elevation.elevation_fractalsum_layers, 0, 10);
	ImGui::SliderFloat("Frequency", &ts.elevation.elevation_frequency, 0, 10);
	ImGui::SliderFloat("Frequency X", &ts.elevation.elevation_frequency_x, 0, 10);
	ImGui::SliderFloat("Frequency Y", &ts.elevation.elevation_frequency_y, 0, 10);
	ImGui::SliderFloat("Frequency Multiplier", &ts.elevation.elevation_frequency_multiplier, 0, 10);
	ImGui::SliderInt("Elevation Offset", &ts.elevation.elevation_offset, 0, 10);
	ImGui::SliderInt("Elevation Range", &ts.elevation.elevation_range, 0, 255);

	if (ImGui::Button("Generate")) {
		gen.generate(ts, width, height, 0, seed);
	}

	ImGui::End();

	ImGui::Begin("Moisture Settings");
	ImGui::SliderFloat("Moisture Offset X", &ts.moisture.moisture_offset_x, 0, 10);
	ImGui::SliderFloat("Moisture Offset Y", &ts.moisture.moisture_offset_y, 0, 10);
	ImGui::SliderFloat("Amplitude Multiplier", &ts.moisture.moisture_amplitude_multiplier, 0, 10);
	ImGui::SliderInt("Fractal Sum Layers", &ts.moisture.moisture_fractalsum_layers, 0, 10);
	ImGui::SliderFloat("Frequency", &ts.moisture.moisture_frequency, 0, 10);
	ImGui::SliderFloat("Frequency X", &ts.moisture.moisture_frequency_x, 0, 10);
	ImGui::SliderFloat("Frequency Y", &ts.moisture.moisture_frequency_y, 0, 10);
	ImGui::SliderFloat("Frequency Multiplier", &ts.moisture.moisture_frequency_multiplier, 0, 10);

	if (ImGui::Button("Generate Moisture")) {
	}

	ImGui::End();

	ImGui::Begin("Save World");

	if (ImGui::Button("Save")) {
		write_tiff(gen, getCurrentDirectory() + "\\world");
	}

	ImGui::End();

	ImGui::Begin("Display");
	ImGui::Checkbox("Elevation", &ds.elevation);
	ImGui::Checkbox("Moisture", &ds.moisture);
	ImGui::Checkbox("Heatmap", &ds.heatmap);
	ImGui::End();

	ImGui::Render();

	//int display_width, display_height;
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main()
{
	TIFFSetWarningHandler(warningHandler);
	TIFFSetErrorHandler(errorHandler);

	glfwSetErrorCallback(glfw_error_callback);

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(1024, 768, format("Legends Preview[%i.%i.%i]", 0, 0, 1).c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);

	glewInit();

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	TerrainSettings ts;
	DisplaySettings ds;

	Generator gen;
	//gen.generate(ts, 256, 256, 0, 2019);

	while (!glfwWindowShouldClose(window))
	{
		render(window, gen, ds);

		render_ui(window, gen, ts, ds);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Shutdown IMGUI
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Shutdown glfw
	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}

