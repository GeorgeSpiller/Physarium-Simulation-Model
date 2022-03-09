#include <glad/glad.h> // include glad to get all thre required opengl hedders
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <time.h> 
#include <chrono>  // for high_resolution_clock
#include <ctgmath> // for rotating 2d vectors
#include <random>

#define DEFAULT_STEP_SIZE 0.02f;

constexpr double PI = 3.14159265358;

/*
	The Agent class, defines a single agent.
	This class is designed such that movment logic is kept to a minimum, so it can instead be
	calculated elsewhere (Compute Shader). Therefore this class's primary goal is to store all
	the data nessesary for a singal agenst movment computation logic.

	The compute shader recieves as input:
	- The agents FL, F and FR sensor values
	- The agents current position

	The Compute shader updates:
	- The agents rotation
	- The agents position
	- The trail map

	The Agent constructor recieves as input
	- Step size
	- deposition amount
	- sensor size
	- sensor distance
	- sensor angle
	- rotation angle
	- deposition amount
*/

class Agent
{
protected:
	static int nextID; // used for incramenting agent ID's
public:
	// public
	// the Agent ID
	unsigned int ID;
	float a_stepSize;
	double a_position[3];
	float a_color[3];
	double a_rotation;

	// constructors
	Agent();
	Agent(const Agent& orig);
	Agent& operator=(const Agent& orig);
	Agent( float stepSize, double rotation);
	// destructor
	~Agent();
	
	// moves the agent forward by SETP_SIZE
	void moveForward();

	void printAgentData();

	void setPositionRandom(int boundX, int boundY);

	void setColorRandom();

	void setColorRandom(int RGBindex);

private:
	// nothing
	void checkBounds();
	double * RotateVector2d(double x, double y, double rad);
	double getRand180Rad();
};

int Agent::nextID = 0;

//Constructors
Agent::Agent() 
{
	// by default the position and color is random
	ID = ++nextID;
	a_stepSize = DEFAULT_STEP_SIZE;

	for (int i = 0; i < 3; i++)
	{
		a_position[i] = 0.0;
		a_color[i] = 0.0f;
	}
	a_rotation = getRand180Rad();
};

Agent::Agent(const Agent& orig) {
	ID = orig.ID;

	a_stepSize = DEFAULT_STEP_SIZE;

	for (int i = 0; i < 3; i++)
	{
		a_position[i] = 0.0;
		a_color[i] = 0.0f;
	}
	a_rotation = 0.0;
}

Agent& Agent::operator=(const Agent& orig) {
	ID = orig.ID;

	a_stepSize = DEFAULT_STEP_SIZE;

	for (int i = 0; i < 3; i++)
	{
		a_position[i] = 0.0;
		a_color[i] = 0.0f;
	}
	a_rotation = 0.0;
	return(*this);
}

Agent::Agent(float stepSize, double rotation)
{
	// by default the position and color is random
	time_t timer;
	ID = (unsigned int) time(&timer);

	a_stepSize = stepSize;

	for (int i = 0; i < 3; i++) 
	{
		a_position[i] = 0.0;
		a_color[i] = 0.0f;
	}

	a_rotation = rotation; 
};

// destructor function
Agent::~Agent()
{
};

void Agent::moveForward()
{	
	// PI = 180deg, 2PI = 360deg
	double* newPosition = RotateVector2d(a_position[0], a_position[1], a_rotation);
	a_position[0] = newPosition[0];
	a_position[1] = newPosition[1];

	checkBounds();
};

void Agent::printAgentData() 
{
	std::cout <<
		"Agent [" << ID << "]:\n" <<
		"\tPosition: (" << a_position[0] << ", " << a_position[1] << "). " <<
		"Rotation: (" << a_rotation << ").\n" <<
		"\tColor: (" << a_color[0] << ", " << a_color[1] << ", " << a_color[2] <<
		"), StepSize: " << a_stepSize <<
		std::endl;
}

void Agent::setPositionRandom(int boundX, int boundY)
{
	double randX = static_cast<double> (rand() % boundX);
	double randY = static_cast<double> (rand() % boundY);
	// normalise positions
	randX = ((randX / static_cast<float>(boundX)) * 2) -1;
	randY = ((randY / static_cast<float>(boundY)) * 2) -1;
	// normalise function:
	// NormalisedValue = ((value / maxValue) * outputMax)

	a_position[0] = randX;
	a_position[1] = randY;
	a_position[2] = 1.0f;
}

void Agent::setColorRandom() 
{
	a_color[0] = (static_cast<float>(rand() % 255) / 255);
	a_color[1] = (static_cast<float>(rand() % 255) / 255);
	a_color[2] = (static_cast<float>(rand() % 255) / 255);
}

void Agent::setColorRandom(int RGBindex)
{
	RGBindex = RGBindex % 3;
	switch (RGBindex) 
	{
	case 0:
		a_color[0] = (static_cast<float>(rand() % 255) / 255) / 4;
		a_color[1] = (static_cast<float>(rand() % 255) / 255) / 4;
		a_color[2] = ((static_cast<float>(rand() % 255) / 255) / 2) + 0.3f;
		break;
	case 1:
		a_color[0] = ((static_cast<float>(rand() % 255) / 255) / 2) + 0.3f;
		a_color[1] = (static_cast<float>(rand() % 255) / 255) / 4;
		a_color[2] = (static_cast<float>(rand() % 255) / 255) / 4;
		break;
	case 2:
		a_color[0] = (static_cast<float>(rand() % 255) / 255) / 4;
		a_color[1] = ((static_cast<float>(rand() % 255) / 255) / 2) + 0.3f;
		a_color[2] = (static_cast<float>(rand() % 255) / 255) / 4;
		break;
	}
}

double * Agent::RotateVector2d(double x, double y, double rad)
{
	static double rotatedVector[2];
	//rotatedVector[0] = x * cos(rad) - y * sin(rad);
	//rotatedVector[1] = x * sin(rad) + y * cos(rad);
	//return rotatedVector;
	
	// newx = x + cos(angle) * distance;
	// newy = y + sin(andle) + dist
	rotatedVector[0] = x + cos(rad) * a_stepSize;
	rotatedVector[1] = y + sin(rad) * a_stepSize;
	return rotatedVector;

}

void Agent::checkBounds()
{
	// wrap around bounds
	if (a_position[0] > 1)
	{	// East
		a_position[0] = -1;
	}
	if (a_position[0] < -1)
	{	// West
		a_position[0] = 1;
	}
	if (a_position[1] > 1)
	{	// South
		a_position[1] = -1;
	}
	else if (a_position[1] < -1)
	{	// North
		a_position[1] = 1;
	}
}

double Agent::getRand180Rad() 
{
	// get random angle between 0-PI rads (0-180 deg)
	std::mt19937 rng(std::random_device{}()); //Unless you are using MinGW
	std::uniform_real_distribution<> dist(0, 2 * PI);
	return dist(rng);
}

/*
	// private


	*  Enum of potential Actions that an agent could take, sorted from most common to least common

	[Motor Stage]
	MOVE_FORWARD		: Deposit weight onto trail map at current location, and move forward by STEP_SIZE
	RAND_ROT			: Rotate in a random direction 360deg

	[Sensor Stage]
	HOLD				: Stay facing in the current direction, do not move forward
	ROT_R				: Rotate right by ROTATION_ANGLE
	ROT_L				: Rotate left by ROTATION_ANGLE
	RAND_LR		: Rotate randomly iether left or right be ROTATION_ANGLE

enum actions { MOVE_FORWARD, RAND_ROT, HOLD, ROT_R, ROT_L, RAND_LR };

// the current position of the (FL) Front Left, (F) Front, (FR) Front Right sensors respectivly, 
// defined as normalised device coordinates x:(-1, 1), y:(-1, 1)
float sensor_FL_position;
float sensor_F_position;
float sensor_FR_position;

// the current rotation of the agent
float rotation;

// ---------- private methods ---------- 
// returns a random rotaion between 0-360 degs
float randRot();

// returns the weight value at the Front Left sensor
void read_FL();
// returns the weight value at the Front sensor
void read_F();
// returns the weight value at the Front Right sensor
void read_FR();

// deposits a trail at the position of the agent
void deposition();

// moves the agent forward by SETP_SIZE
void moveForward();

*/