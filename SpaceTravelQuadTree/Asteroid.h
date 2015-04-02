#pragma once 

// Asteroid class.
class Asteroid
{
public:
   Asteroid();
   Asteroid(float x, float y, float z, float r, unsigned char valueR, 
	    	unsigned char valueG, unsigned char valueB);
   float getCenterX() { return centerX; }
   float getCenterY() { return centerY; }
   float getCenterZ() { return centerZ; }
   float getRadius()  { return radius; }
   void draw();
private:
   float centerX, centerY, centerZ, radius;
   unsigned char color[3];
};
