#include "PrePatterning.h"

PrePatterning::PrePatterning(std::vector<Stimuli> inputStimuli, size_t numStimuli)
{
	stimuli.resize(numStimuli);
	
	for (size_t i = 0; i < numStimuli; i++)
	{
		stimuli.at(i) = { inputStimuli[i].size, inputStimuli[i].pos_x,inputStimuli[i].pos_y };
	}

}

void PrePatterning::addStimuliNode(Stimuli newStimuli) 
{
	numStimuli += 1;
	stimuli.resize(numStimuli);
	stimuli.at(numStimuli-1) = { newStimuli.size, newStimuli.pos_x, newStimuli.pos_y };
}



