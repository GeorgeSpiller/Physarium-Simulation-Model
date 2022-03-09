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
	AgentSim(int width, int height, size_t numAgents) : numAgents(numAgents)
	{

		std::mt19937 randGen;
		std::uniform_real_distribution<float> position_x(0.0f, (float)width);
		std::uniform_real_distribution<float> position_y(0.0f, (float)height);
		std::uniform_real_distribution<float> angle(0.0f, TWO_PI);

		agents.resize(numAgents);

		for (size_t i = 0; i < numAgents; i++)
		{
			agents.at(i) = { position_x(randGen), position_y(randGen), angle(randGen) };
		}
	}

	const std::vector<Agent>& getAgents() const { return agents; }

private:
	size_t numAgents;
	std::vector<Agent> agents;
};




