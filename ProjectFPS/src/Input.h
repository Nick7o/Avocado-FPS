#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Event.h"

class Input
{
public:
	static Event<glm::vec2> MouseMoved;
	static Event<int, bool> KeyChange;
	static Event<int, bool> MouseButtonChange;

	static bool* GetKeys() { return keys; }

	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* window, double xPosition, double yPosition);
	static void HandleMouseButtons(GLFWwindow* window, int button, int action, int mods);

private:
	static bool keys[1024];

	static glm::vec2 lastMousePosition;
	static glm::vec2 mousePositionChange;
};

