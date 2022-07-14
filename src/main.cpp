// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "Logger.hpp"
#include "Input.hpp"
#include "Callbacks.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Sphere.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define MENU

constexpr int width = 1200, height = 900;
float ColorOffset{};
float heightMultiplier{1.0f};
float lastX = width / 2.0f;
float lastY = height / 2.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	const float xpos = static_cast<float>(xPos);
	const float ypos = static_cast<float>(yPos);

	if (camera.FirstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		camera.FirstMouse = false;
	}

	const float xOffset = xpos - lastX;
	const float yOffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouse(xOffset, yOffset);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	static bool isMouseToggled{false};
	static bool isWireFrameToggled{false};
	if (action == GLFW_PRESS)
	{

		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			return;
		case GLFW_KEY_1:
			if (isWireFrameToggled) 
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				isWireFrameToggled = false;
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				isWireFrameToggled = true;
			}
			return;
		case GLFW_KEY_TAB:
			if (isMouseToggled)
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				isMouseToggled = !isMouseToggled;
				camera.FirstMouse = true;
				camera.useMouse = !camera.useMouse;
			}
			else
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				isMouseToggled = !isMouseToggled;
				camera.useMouse = !camera.useMouse;
			}
			return;
		default:
			return;
		}
	}
}

GLFWwindow* initOpenGL()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create the window object
	GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		LogError("Failed to create GLFW window\n");
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LogError("Failed to initialize GLAD\n");
		return nullptr;
	}

	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetKeyCallback(window, keyCallback);
	return window;
}

void getHardwareStats()
{
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	LogInfo("Maximum number of vertex attributes allowed on hardware: " + std::to_string(nrAttributes) + "\n");
}

#define RenderMenu() \
	ImGui_ImplOpenGL3_NewFrame(); \
	ImGui_ImplGlfw_NewFrame(); \
	ImGui::NewFrame(); \
	ImGui::Begin("Properties"); \
	{ \
		ImGui::SliderFloat("Color Offset", &ColorOffset, -2.0f, 2.0f, "%.2f"); \
		ImGui::SliderFloat("heightMultiplier", &heightMultiplier, 0.1f, 50.f, "%.2f"); \
		ImGui::SliderFloat("angle", &angle, -180.0f, 180.0f, "%.2f"); \
		ImGui::SliderFloat3("axis", axis, 0.0f, 1.0f, "%.2f"); \
		ImGui::Text("Triangles: %i", sphere.getIndexCount() / 3); \
		ImGui::Text("Vertices: %i", sphere.getInterleavedVertexCount() / 3); \
	} \
	ImGui::End(); \
	ImGui::EndFrame()


int main()
{
	auto window = initOpenGL();
	if (window == nullptr) return -1;

	getHardwareStats();

	Shader ShaderProgram(R"(triangle.vert)", R"(triangle.frag)");
	ShaderProgram.CompileShaders();
	ShaderProgram.CreateAndLinkProgram();

	Sphere sphere(30, 2880, 1440, true);

	//create a vertex buffer object and the vertex array object
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//bind the VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sphere.getInterleavedVertexSize(), sphere.getInterleavedVertices(), GL_STATIC_DRAW);

	// set the vertex attrib pointer up
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sphere.getInterleavedStride(), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sphere.getInterleavedStride(), (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sphere.getInterleavedStride(), (void*)(sizeof(float) * 6));

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.getIndexSize(), sphere.getIndices(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//unbind VAO before unbinding anything else to avoid opengl deassociating it with the vao
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glCheckError();

	unsigned int VAOplane, VBOplane;
	glGenVertexArrays(1, &VAOplane);
	glBindVertexArray(VAOplane);
	glGenBuffers(1, &VBOplane);

	constexpr float verticesPlane[] = 
	{
	     0.5f,  0.5f, 0.0f,  
	     0.5f, -0.5f, 0.0f,  
	    -0.5f,  0.5f, 0.0f, 
	     0.5f, -0.5f, 0.0f,  
	    -0.5f, -0.5f, 0.0f,  
	    -0.5f,  0.5f, 0.0f   
	}; 

	//bind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOplane);
	glBufferData(GL_ARRAY_BUFFER, sizeof verticesPlane, verticesPlane, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glCheckError();

	//add textures
	Texture texture;
	texture.AddTexture(R"(F:\prj\C++\GeoWizard\src\res\heightmap-small.png)", 0);

	glCheckError();

	// set the texture ids of the diffuse and specular maps
	ShaderProgram.UseProgram();

	glCheckError();

	// this doesn't need to be done every frame
	const glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.01f, 100.0f);
	ShaderProgram.SetMat4("projection", projection);
	ShaderProgram.SetInt("tex", 0);

	float lastFrameTime{};
	float timeAccumulator{};
	float fpsAccumulator{};
	unsigned int dataPoints{};

	glCheckError();
#ifdef MENU
	ImGui::CreateContext();
	ImGui_ImplOpenGL3_Init(); // Must be below create context
	ImGui_ImplGlfw_InitForOpenGL(window, true);
#endif
	float angle{-90.0f};
	float axis[3]{1.0f, 1.0f, 1.0f};
	while (!glfwWindowShouldClose(window))
	{
#ifdef MENU
		RenderMenu();
		ShaderProgram.SetVec3("ColorOffset", glm::vec3(ColorOffset));
		ShaderProgram.SetFloat("heightMultiplier", heightMultiplier);
#endif
		const float currentFrame = glfwGetTime();
		const float deltaTime = currentFrame - lastFrameTime;
		lastFrameTime = currentFrame;
		timeAccumulator += deltaTime;

		// calculate fps
		if (timeAccumulator >= 1.0f)
		{
			const float fps = 1.0f / deltaTime;
			std::string title = "LearnOpenGL | FPS: " + std::to_string(fps) + " frametime: " + std::to_string(deltaTime * 1000);
			glfwSetWindowTitle(window, title.c_str());
			fpsAccumulator += fps;
			++dataPoints;
			timeAccumulator = 0.0f;
		}
		
		camera.ProcessKeyboard(window, deltaTime);

		// set the background color to a very deep grey
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		ShaderProgram.UseProgram();
		const glm::mat4 view = camera.RetrieveLookAt();
		ShaderProgram.SetMat4("view", view);

		// set the vao just before drawing
		glBindVertexArray(VAO);
		// section to draw the main cube
		{
			auto model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(angle), {axis[0], axis[1], axis[2]});
			ShaderProgram.SetMat3("modelMatrix", transpose(inverse(model)));
			ShaderProgram.SetMat4("model", model);
			//glDrawArrays(GL_TRIANGLES, 0, faceGenerator.vertices.size());
			texture.BindTexture(0);
			glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, (void*)0);
			Texture::UnbindTexture(0);

		}
		glBindVertexArray(0);

#ifdef MENU
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

#ifdef MENU
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
#endif

	const float averageFPS = fpsAccumulator / dataPoints;
	LogPerf("AVG FPS during " + std::to_string(dataPoints) + " second run: " + std::to_string(averageFPS) + " and an average frametime of " + std::to_string((1.0f / averageFPS) * 1000) + "ms\n");
	glCheckError();
	glfwTerminate();
	return 0;
}
