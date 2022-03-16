#include "Agent.h"



AgentSim::AgentSim(int width, int height, size_t numAgents) : numAgents(numAgents)
	{
		std::mt19937 randGen;
		std::uniform_real_distribution<float> position_x(0.0f, (float)width);
		std::uniform_real_distribution<float> position_y(0.0f, (float)height);
		std::uniform_real_distribution<float> angle(0.0f, TWO_PI);
		const float constAngle = (TWO_PI / 2) + (TWO_PI / 4) / 2;
		/*
		North:	(TWO_PI / 4)     || 
		East:	(TWO_PI / 4) * 4 || TWO_PI		|| 0.0f
		South:	(TWO_PI / 4) * 3 || 
		West:	(TWO_PI / 4) * 2 || TWO_PI / 2
		
		TWO_PI || 0 = 90deg;
		*/

		agents.resize(numAgents);

		for (size_t i = 0; i < numAgents; i++)
		{
			agents.at(i) = { position_x(randGen), position_y(randGen), constAngle }; // constAngle  angle(randGen)
		}
	}




