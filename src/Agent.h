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

class AgentSim
{
public:
	AgentSim() : numAgents(0) {}
	AgentSim(int width, int height, size_t numAgents);

	const std::vector<Agent>& getAgents() const { return agents; }

private:
	size_t numAgents;
	std::vector<Agent> agents;
};




