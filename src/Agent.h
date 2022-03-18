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
	RANDOM = 0, CIRCLE = 1, POINT = 2, POINT2 = 3, POINT4 = 4
};

class AgentSim
{
public:
	AgentSim() : numAgents(0) {}
	AgentSim(int width, int height, size_t numAgents, SpawnMode spawn);

	const std::vector<Agent>& getAgents() const { return agents; }

private:
	size_t numAgents;
	std::vector<Agent> agents;
};




