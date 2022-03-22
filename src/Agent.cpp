#include "Agent.h"
#include <iostream>

AgentSim::AgentSim(int width, int height, size_t numAgents, SpawnMode spawn) : numAgents(numAgents)
	{
		std::mt19937 randGen;
		std::uniform_real_distribution<float> position_x(0.0f, (float)width);
		std::uniform_real_distribution<float> position_y(0.0f, (float)height);
		std::uniform_real_distribution<float> angle(0.0f, TWO_PI);

		agents.resize(numAgents);

		switch(spawn)
		{
		case SpawnMode::RANDOM:
		{
			for (size_t i = 0; i < numAgents; i++)
			{
				agents.at(i) = { position_x(randGen), position_y(randGen), angle(randGen) };
			}
		}
		break;
		case SpawnMode::CIRCLE:
		{
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
		}
		break;
		case SpawnMode::POINT:
		{
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
			float pos_p1_x = width * 0.3f;
			float pos_p2_x = width * 0.6f;
			float pos_y = height * 0.5f;

			for (size_t i = 0; i < numAgents/2; i++)
			{
				agents.at(i) = { pos_p1_x, pos_y, angle(randGen) };
			}
			for (size_t i = numAgents / 2; i < numAgents; i++)
			{
				agents.at(i) = { pos_p2_x, pos_y, angle(randGen) };
			}
		}
		break;
		case SpawnMode::POINT4:
		{
			size_t quaterNumAgents = numAgents / 4;
			float pos_p1_x = width * 0.3f;
			float pos_p2_x = width * 0.6f;
			float pos_p1_y = height * 0.3f;
			float pos_p2_y = height * 0.6f;

			for (size_t i = 0; i < quaterNumAgents; i++)
			{
				agents.at(i) = { pos_p1_x, pos_p1_y, angle(randGen) };
			}
			for (size_t i = quaterNumAgents; i < quaterNumAgents * 2; i++)
			{
				agents.at(i) = { pos_p2_x, pos_p1_y, angle(randGen) };
			}
			for (size_t i = quaterNumAgents * 2; i < quaterNumAgents * 3; i++)
			{
				agents.at(i) = { pos_p1_x, pos_p2_y, angle(randGen) };
			}
			for (size_t i = quaterNumAgents * 3; i < numAgents; i++)
			{
				agents.at(i) = { pos_p2_x, pos_p2_y, angle(randGen) };
			}
		}
		break;
		case SpawnMode::RECT:
		{
			std::cout << "rect" << std::endl;
			float rect_width = width * 0.5f;
			float rect_height = height * 0.2f;
			float pos_x = 0.0f, pos_y = 0.0f;
			for (size_t i = 0; i < numAgents; i++)
			{
				do
				{
					pos_x = position_x(randGen);
					pos_y = position_y(randGen);
				} while (
					pos_x < (width - rect_width) / 2 ||
					pos_x > ((width - rect_width) / 2) + rect_width ||
					pos_y < (height - rect_height) / 2 ||
					pos_y >((height - rect_height) / 2) + rect_height
					);

				agents.at(i) = { pos_x, pos_y, angle(randGen) };
			}
		}
		break;
		default:
			break;
		}
		
	}





