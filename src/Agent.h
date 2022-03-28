#include <vector>
#include <random>
#include <algorithm>

const float TWO_PI = 6.2831853f;


struct Agent
{
	float x;
	float y;
	float angle;
};

enum class SpawnMode
{
	RANDOM, CIRCLE, POINT, POINT2, POINT4, RECT
};

class AgentSim
{
public:
	AgentSim() : numAgents(0) {};
	AgentSim(int width, int height, size_t numAgents, SpawnMode spawn);

	const std::vector<Agent>& getAgents() const { return agents; };

private:
	size_t numAgents;
	std::vector<Agent> agents;
};




