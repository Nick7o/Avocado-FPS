#include "pch.h"
#include "Input.h"

#include "GLFW/glfw3.h"

Event<glm::vec2> Input::MouseMoved;
/* Keycode + is down */
Event<int, bool> Input::KeyChange;
/* Mouse button + is down */
Event<int, bool> Input::MouseButtonChange;

bool Input::keys[1024];
glm::vec2 Input::lastMousePosition = glm::vec2(0.0f, 0.0f);
glm::vec2 Input::mousePositionChange;

void Input::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
			KeyChange.Call(key, true);
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
			KeyChange.Call(key, false);
		}
	}
}

void Input::HandleMouse(GLFWwindow* window, double xPosition, double yPosition)
{
	static bool firstMove = true;
	if (firstMove)
	{
		lastMousePosition = glm::vec2(xPosition, yPosition);
		firstMove = false;
	}

	double xChange = xPosition - lastMousePosition.x;
	double yChange = lastMousePosition.y - yPosition;

	mousePositionChange = glm::vec2(xChange, yChange);
	lastMousePosition = glm::vec2(xPosition, yPosition);

	MouseMoved.Call(mousePositionChange);
}

void Input::HandleMouseButtons(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		MouseButtonChange.Call(button, true);
	}
	else if (action == GLFW_RELEASE)
	{
		MouseButtonChange.Call(button, false);
	}
}
