//////////////////////////////////////////////////////////////////////////////////////         
// spaceTravelFrustumCulled.cpp
//
// This program is based on spaceTravel.cpp with an added frustum culling option.
// It draws a conical spacecraft that can travel and an array of fixed spherical 
// asteroids. The view in the left viewport is from a fixed camera; the view in 
// the right viewport is from the spacecraft.There is approximate collision detection.  
// Frustum culling is implemented by means of a quadtree data structure.
// 
// COMPILE NOTE: File intersectionDetectionRoutines.cpp must be in the same folder.
// EXECUTION NOTE: If ROWS and COLUMNS are large the quadtree takes time to build so
//                 the display may take several seconds to come up.
//
// User-defined constants: 
// ROWS is the number of rows of  asteroids.
// COLUMNS is the number of columns of asteroids.
// FILL_PROBABILITY is the percentage probability that a particular row-column slot
// will be filled with an asteroid.
//
// Interaction:
// Press the left/right arrow keys to turn the craft.
// Press the up/down arrow keys to move the craft.
// Press space to toggle between frustum culling enabled and disabled.
// 
// Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////// 
#include <ctime> 
#include <cmath>
#include <iostream>

#include "intersectionDetectionRoutines.h"
#include "Asteroid.h"
#include "Renderer.h"
#include "Input.h"

using namespace std;

// Globals.
static float angle = 0.0; // Angle of the spacecraft.
static float xVal = 0, zVal = 0; // Co-ordinates of the spacecraft.
static int isFrustumCulled = 0;
static int isCollision = 0; // Is there collision between the spacecraft and an asteroid?
static float speed, angSpeed;
static float tempxVal, tempzVal, tempAngle;


// the asteroids and quad tree from the initial program
Asteroid **arrayAsteroids; // Global array of asteroids.

// Initialization routine.
void setup(void)
{
	Renderer& renderer = Renderer::getInstance();

	int i, j;
	// create meory for each potential asteroid
	arrayAsteroids = new Asteroid *[ROWS];
	for (int i = 0; i < ROWS; i++) {
		arrayAsteroids[i] = new Asteroid[COLUMNS];
	}

	renderer.createLine();

	renderer.createCone();

	renderer.createSphere();

	// Initialize global arrayAsteroids.
	for (j = 0; j < COLUMNS; j++)
		for (i = 0; i < ROWS; i++)
			if (rand() % 100 < FILL_PROBABILITY)
				// If rand()%100 >= FILL_PROBABILITY the default constructor asteroid remains in the slot which
				// indicates that there is no asteroid there because the default's radius is 0.
			{
				// Position the asteroids depending on if there is an even or odd number of columns
				// so that the spacecraft faces the middle of the asteroid field.
				int oddevenOffset = (COLUMNS % 2) ? 0.0 : 15.0;

				arrayAsteroids[i][j] = Asteroid(oddevenOffset + 30.0*(-COLUMNS / 2 + j), 0.0, -40.0 - 30.0*i, 3.0,
					rand() % 256, rand() % 256, rand() % 256);
			}

	renderer.createBuffers();
}

// Function to check if two spheres centered at (x1,y1,z1) and (x2,y2,z2) with
// radius r1 and r2 intersect.
int checkSpheresIntersection(float x1, float y1, float z1, float r1,
	float x2, float y2, float z2, float r2)
{
	return ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2) <= (r1 + r2)*(r1 + r2));
}

// Function to check if the spacecraft collides with an asteroid when the center of the base
// of the craft is at (x, 0, z) and it is aligned at an angle a to to the -z direction.
// Collision detection is approximate as instead of the spacecraft we use a bounding sphere.
int asteroidCraftCollision(float x, float z, float a)
{
	int i, j;

	// Check for collision with each asteroid.
	for (j = 0; j < COLUMNS; j++)
		for (i = 0; i < ROWS; i++)
			if (arrayAsteroids[i][j].getRadius() > 0) // If asteroid exists.
				if (checkSpheresIntersection(x - 5 * sin((PI / 180.0) * a), 0.0,
					z - 5 * cos((PI / 180.0) * a), 7.072,
					arrayAsteroids[i][j].getCenterX(), arrayAsteroids[i][j].getCenterY(),
					arrayAsteroids[i][j].getCenterZ(), arrayAsteroids[i][j].getRadius()))
					return 1;
	return 0;
}

void update()
{
	Input& input = Input::getInstance();

	speed = angSpeed = 0;

	if (input.getKey(KEYCODE_DOWN))
	{
		speed += 1;
	}

	if (input.getKey(KEYCODE_UP))
	{
		speed -= 1;
	}

	if (input.getKey(KEYCODE_LEFT))
	{
		angSpeed += 1;
	}

	if (input.getKey(KEYCODE_RIGHT))
	{
		angSpeed -= 1;
	}

	tempAngle = angle + angSpeed * 2.0;

	// Angle correction.
	if (tempAngle > 360.0) tempAngle -= 360.0;
	if (tempAngle < 0.0) tempAngle += 360.0;

	tempxVal = xVal + speed * sin(tempAngle * PI / 180.0);
	tempzVal = zVal + speed * cos(tempAngle * PI / 180.0);

	// Move spacecraft to next position only if there will not be collision with an asteroid.
	if (!asteroidCraftCollision(tempxVal, tempzVal, tempAngle))
	{
		isCollision = 0;
		xVal = tempxVal;
		zVal = tempzVal;
		angle = tempAngle;
	}
	else
	{
		tempxVal = xVal;
		tempzVal = zVal;
		tempAngle = angle;
		isCollision = 1;
	}
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	cout << "ALERT: The OpenGL window may take a while to come up because" << endl
		<< "of the time to build the quadtree!" << endl
		<< endl;
	cout << "Interaction:" << endl;
	cout << "Press the left/right arrow keys to turn the craft." << endl
		<< "Press the up/down arrow keys to move the craft." << endl
		<< "Press space to toggle between frustum culling enabled and disabled." << endl;
}

// Main routine.
int main(int argc, char **argv)
{

	srand((unsigned)time(0));
	printInteraction();

	Renderer& renderer = Renderer::getInstance();
	Input& input = Input::getInstance();

	renderer.start();
	input.start();

	// init the graphics and rest of the app
	setup();

	// run!
	while (!renderer.isDisposed())
	{
		update();

		// Flush the InputManager at the end of every frame
		input.flush();

		renderer.draw(arrayAsteroids, xVal, zVal, angle);
	}


	return 0;

}

