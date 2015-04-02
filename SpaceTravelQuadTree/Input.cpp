#include "Input.h"

Input::~Input()
{
	delete keysDown;
	delete keysUp;
	delete keysPressed;
}

void Input::start()
{
	keysDown = new unordered_set<KeyCode>();
	keysUp = new unordered_set<KeyCode>();
	keysPressed = new unordered_set<KeyCode>();

	glfwSetKeyCallback(Renderer::getInstance().getWindow(), _inputCallback);
}

bool Input::getKeyDown(KeyCode key)
{
	return keysDown->find(key) != keysDown->end();
}

bool Input::getKey(KeyCode key)
{
	return getKeyDown(key) || keysPressed->find(key) != keysPressed->end();
}

bool Input::getKeyUp(KeyCode key)
{
	return keysUp->find(key) != keysUp->end();
}

void Input::flush()
{
	// persist all keys pressed at this frame.
	for (const KeyCode &key : *keysDown)
	{
		keysPressed->insert(key);
	}

	// clear keys down and up
	keysDown->clear();
	keysUp->clear();
}


void Input::onKeyDown(int key)
{
	KeyCode code = getKeyCodeFromGLKey(key);

	keysDown->insert(code);
}

void Input::onKeyUp(int key)
{
	KeyCode code = getKeyCodeFromGLKey(key);

	// remove the key from pressed keys
	keysPressed->erase(code);

	// and add it to up keys
	keysUp->insert(code);
}

// map all keys used in the game
KeyCode Input::getKeyCodeFromGLKey(int key)
{
	switch (key) {
	case GLFW_KEY_ESCAPE:
		return KEYCODE_ESCAPE;
	case GLFW_KEY_SPACE:
		return KEYCODE_SPACE;
	case GLFW_KEY_LEFT:
		return KEYCODE_LEFT;
	case GLFW_KEY_RIGHT:
		return KEYCODE_RIGHT;
	case GLFW_KEY_UP:
		return KEYCODE_UP;
	case GLFW_KEY_DOWN:
		return KEYCODE_DOWN;
	default:
		return KEYCODE_NONE;
	}
}

void Input::_inputCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
		Input::getInstance().onKeyDown(key);
		break;
	case GLFW_RELEASE:
		Input::getInstance().onKeyUp(key);
		break;
	}
}