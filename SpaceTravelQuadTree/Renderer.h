#pragma once

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "Asteroid.h"

using namespace std;

#pragma comment ( lib, "opengl32.lib" )
#pragma comment ( lib, "glew32.lib" )
#pragma comment ( lib, "glfw3.lib" )

#define ROWS 100  // Number of rows of asteroids.
#define COLUMNS 100 // Number of columns of asteroids.
#define FILL_PROBABILITY 100 // Percentage probability that a particular row-column slot will be 

// filled with an asteroid. It should be an integer between 0 and 100.
#define WINDOW_X 1600
#define WINDOW_Y 800

// vertex counting for where everything goes in the global array
// fixed number of vertices for cone and sphere
#define CONE_VERTEX_COUNT 12
#define LINE_VERTEX_COUNT 2
#define SPHERE_VERTEX_COUNT 288 // moved to Asteroids.h because asteroids draw themselves

#define SPHERE_SIZE 5.0f

#define PI 3.14159265

class Renderer
{
public:
	int start();
	GLFWwindow* getWindow();
	void draw(Asteroid** asteroids, float x, float z, float angle);
	void drawSphere(float x, float y, float z, unsigned char* color);

	bool isDisposed();

	void createBuffers();
	void createLine();
	void createCone();
	void createSphere();

	~Renderer();
	static Renderer& getInstance() {
		static Renderer instance;
		return instance;
	}
private:
	GLFWwindow* window;

	// window size
	int width;
	int height;

	int cone_index = 0;
	int line_index = cone_index + CONE_VERTEX_COUNT;
	int sphere_index = line_index + LINE_VERTEX_COUNT;

	void createConeMesh(const glm::vec3 &d, const glm::vec3 &a,
		const float h, const float rd, const int n, int offset);
	void createSphereMesh(const float R, const float H, const float K, const float Z, int offset);
	glm::vec3 perp(const glm::vec3 &v);

	// shader stuff
	glm::vec3 points[CONE_VERTEX_COUNT + LINE_VERTEX_COUNT + SPHERE_VERTEX_COUNT*ROWS*COLUMNS]; // addition of all rows/cols for asteroid vertices + spaceship vertices + line vertices  
	GLuint  myShaderProgram;
	GLuint	myBuffer;
	char* readShaderSource(const char* shaderFile);
	GLuint initShaders(const char* vShaderFile, const char* fShaderFile);

	void lookAt(
		float eyex,		float eyey,		float eyez, 
		float centerx,	float centery,	float centerz,
		float upx,		float upy,		float upz);
	void resize(int w, int h);

	// Static callbacks
	static void _resizeCallback(GLFWwindow* window, int w, int h);

	Renderer() {}
	Renderer(Renderer& const);
	void operator=(Renderer& const);
};

