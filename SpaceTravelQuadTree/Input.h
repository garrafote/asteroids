#pragma once

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <unordered_set>

#include "Renderer.h"

using namespace std;

enum KeyCode;

class Input
{
public:
	bool getKeyDown(KeyCode key);
	bool getKeyUp(KeyCode key);
	bool getKey(KeyCode key);
	void flush();
	void start();

	~Input();
	static Input& getInstance() {
		static Input instance;
		return instance;
	}

private:
	unordered_set<KeyCode> *keysDown;
	unordered_set<KeyCode> *keysUp;
	unordered_set<KeyCode> *keysPressed;

	void onKeyDown(int key);
	void onKeyUp(int key);
	KeyCode getKeyCodeFromGLKey(int key);

	// Static callbacks
	static void _inputCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	Input() {}
	Input(Input const&);
	void operator=(Input const&);
};

// this enum was borowed form another project of mine.
enum KeyCode
{
	KEYCODE_NONE		= 0x00,

	KEYCODE_ESCAPE		= 0x1B, // ESC key
	KEYCODE_CONVERT		= 0x1C, // IME convert
	KEYCODE_NONCONVERT	= 0x1D, // IME nonconvert
	KEYCODE_ACCEPT		= 0x1E, // IME accept
	KEYCODE_MODECHANGE	= 0x1F, // IME mode change request
	KEYCODE_SPACE		= 0x20, // SPACEBAR
	KEYCODE_PRIOR		= 0x21, // PAGE UP key
	KEYCODE_NEXT		= 0x22, // PAGE DOWN key
	KEYCODE_END			= 0x23, // END key
	KEYCODE_HOME		= 0x24, // HOME key
	KEYCODE_LEFT		= 0x25, // LEFT ARROW key
	KEYCODE_UP			= 0x26, // UP ARROW key
	KEYCODE_RIGHT		= 0x27, // RIGHT ARROW key
	KEYCODE_DOWN		= 0x28, // DOWN ARROW key
	KEYCODE_SELECT		= 0x29, // SELECT key
	KEYCODE_PRINT		= 0x2A, // PRINT key
	KEYCODE_EXECUTE		= 0x2B, // EXECUTE key
	KEYCODE_SNAPSHOT	= 0x2C, // PRINT SCREEN key
	KEYCODE_INSERT		= 0x2D, // INS key
	KEYCODE_DELETE		= 0x2E, // DEL key
	KEYCODE_HELP		= 0x2F, // HELP key
	
	// 0 - 9 keys
	KEYCODE_0 = 0x30,
	KEYCODE_1 = 0x31,
	KEYCODE_2 = 0x32,
	KEYCODE_3 = 0x33,
	KEYCODE_4 = 0x34,
	KEYCODE_5 = 0x35,
	KEYCODE_6 = 0x36,
	KEYCODE_7 = 0x37,
	KEYCODE_8 = 0x38,
	KEYCODE_9 = 0x39,

	// A - Z keys
	KEYCODE_A = 'A', // A Key
	KEYCODE_B = 'B', // B Key
	KEYCODE_C = 'C', // C Key
	KEYCODE_D = 'D', // D Key
	KEYCODE_E = 'E', // E Key
	KEYCODE_F = 'F', // F Key
	KEYCODE_G = 'G', // G Key
	KEYCODE_H = 'H', // H Key
	KEYCODE_I = 'I', // I Key
	KEYCODE_J = 'J', // J Key
	KEYCODE_K = 'K', // K Key
	KEYCODE_L = 'L', // L Key
	KEYCODE_M = 'M', // M Key
	KEYCODE_N = 'N', // N Key
	KEYCODE_O = 'O', // O Key
	KEYCODE_P = 'P', // P Key
	KEYCODE_Q = 'Q', // Q Key
	KEYCODE_R = 'R', // R Key
	KEYCODE_S = 'S', // S Key
	KEYCODE_T = 'T', // T Key
	KEYCODE_U = 'U', // U Key
	KEYCODE_V = 'V', // V Key
	KEYCODE_W = 'W', // W Key
	KEYCODE_X = 'X', // X Key
	KEYCODE_Y = 'Y', // Y Key
	KEYCODE_Z = 'Z', // Z Key
};