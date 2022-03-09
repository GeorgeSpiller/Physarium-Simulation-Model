#include <glad/glad.h> // include glad to get all thre required opengl hedders
#include <string>
#include <iostream>]
#include <random>


class TrailMap
{
public:
	TrailMap(int window_width, int window_height);

	void depositTrail(double x, double y, double weight, int channel);
private:
	int WIN_X;
	int WIN_Y;
	static double* TrailMapArray;
};

TrailMap::TrailMap(int window_width, int window_height) 
{
	WIN_X = window_width;
	WIN_Y = window_height;
	const int TrailMapX = WIN_X;
	const int TrailMapY = WIN_Y;

	//TrailMapArray = new double[TrailMapX][TrailMapY][4];

	// normalise coord = ( (input - max) / (max - min) ) * maxOut
}

TrailMap::~TrailMap() 
{
	delete TrailMapArray;
}

void TrailMap::depositTrail(double x, double y, double weight, int channel) 
{
	
}
