// Legends.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Generator/Generator.h"

#include "tiffio.h"

#include "GL/Camera.h"
#include "GL/Display.h"
#include "GL/Mesh.h"
#include "GL/Texture.h"

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_callback(GLFWwindow *window, int button, int action, int mods) {
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main()
{
	TIFFSetWarningHandler(warningHandler);
	TIFFSetErrorHandler(errorHandler);

	glfw_init();

	GLFWwindow *window = glfwCreateWindow(1024, 768, format("Legends Preview[%i.%i.%i]", 0, 0, 1).c_str(), nullptr, nullptr);

	if (window == nullptr) {
		printf("Unable to create window\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	
	glewInit();

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	TerrainSettings ts;
	DisplaySettings ds;
	DisplayObjects objs;

	Generator gen;

	object_init(gen, objs);

	while (!glfwWindowShouldClose(window)) {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		resize(objs, width, height);

		render(window, gen, ds, objs);

		render_ui(window, gen, ts, ds, objs);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	object_cleanup(objs);

	// Shutdown IMGUI
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Shutdown glfw
	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}

