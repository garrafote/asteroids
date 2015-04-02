#include "Asteroid.h"
#include "Renderer.h"

using namespace std;

// Asteroid default constructor.
Asteroid::Asteroid()
{
   centerX = 0.0;
   centerY = 0.0;
   centerZ = 0.0; 
   radius = 0.0; // Indicates no asteroid exists in the position.
   color[0] = 0;
   color[1] = 0;
   color[2] = 0;
}

// Asteroid constructor.
Asteroid::Asteroid(float x, float y, float z, float r, unsigned char valueR, 
		unsigned char valueG, unsigned char valueB)
{
   centerX = x;
   centerY = y;
   centerZ = z; 
   radius = r;
   color[0] = valueR;
   color[1] = valueG;
   color[2] = valueB;
}

// Function to draw asteroid.
void Asteroid::draw()
{
   if (radius > 0.0) // If asteroid exists.
   {
	   Renderer::getInstance().drawSphere(centerX, centerY, centerZ, color);
   }
}
