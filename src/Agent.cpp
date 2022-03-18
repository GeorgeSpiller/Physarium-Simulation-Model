#include "Agent.h"
#include <iostream>

AgentSim::AgentSim(int width, int height, size_t numAgents, SpawnMode spawn) : numAgents(numAgents)
	{
		std::mt19937 randGen;
		std::uniform_real_distribution<float> position_x(100.0f, (float)width - 100.0f);
		std::uniform_real_distribution<float> position_y(100.0f, (float)height - 100.0f);
		std::uniform_real_distribution<float> angle(0.0f, TWO_PI);

		// If not using random distribution
		const float constAngle = (TWO_PI / 2) + (TWO_PI / 4) / 2;
		const float constPosition_x = float(width) / 2;
		const float constPosition_y = float(height) / 2;

		agents.resize(numAgents);

		std::cout << "SpawnMode: " << int(spawn) << std::endl; // 0: RANDOM, 1: CIRCLE, 2:POINT .. ect
		switch(spawn)
		{
		case SpawnMode::RANDOM:
		{
			std::cout << "Random: " << int(spawn) << std::endl;
			for (size_t i = 0; i < numAgents; i++)
			{
				agents.at(i) = { position_x(randGen), position_y(randGen), angle(randGen) };
			}
		}
		break;
		case SpawnMode::CIRCLE:
		{
			std::cout << "circle: " << int(spawn) << std::endl;
			if (spawn == SpawnMode::RANDOM) 
			{
				// absolutly no idea why 
			}
			
			std::cout << std::endl;
			const float radius = std::min(width, height) / 3.0f;
			float pos_x, pos_y;
			for (size_t i = 0; i < numAgents; i++)
			{
				do
				{
					pos_x = position_x(randGen);
					pos_y = position_y(randGen);
				} while (powf(pos_x - width * 0.5f, 2) + powf(pos_y - height * 0.5f, 2) > (radius * radius));
				agents.at(i) = { pos_x, pos_y, angle(randGen) };
			}
			break;
		}
		case SpawnMode::POINT:
		{
			std::cout << "point" << std::endl;
			float pos_x = width * 0.5f;
			float pos_y = height * 0.5f;
			for (size_t i = 0; i < numAgents; i++)
			{
				agents.at(i) = { pos_x, pos_y, angle(randGen) };
			}
		}
		break;
		case SpawnMode::POINT2:
		{
			std::cout << "point2" << std::endl;
		}
		break;
		case SpawnMode::POINT4:
		{
			std::cout << "point4" << std::endl;

		}
		default:
			break;
		}
		
	}





