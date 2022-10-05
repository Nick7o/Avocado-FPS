#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class OpenGLWindow
{
public:
	OpenGLWindow(unsigned int width, unsigned int height);
	~OpenGLWindow();

	bool Initialize();
	void SwapBuffers();

	inline bool ShouldClose() const { return glfwWindowShouldClose(window); }

	inline GLFWwindow* GetWindowPointer() const { return window; }
	inline unsigned int GetWidth() const { return width; }
	inline unsigned int GetHeight() const { return height; }

private:
	GLFWwindow* window;
	unsigned int width, height;

	void SetInputCallbacks();

	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
};