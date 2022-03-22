#include <vector>

struct Stimuli
{
	float size;
	float pos_x;
	float pos_y;
};


class PrePatterning 
{
public:
	PrePatterning() : numStimuli(0) {}
	PrePatterning(std::vector<Stimuli> inputStimuli,  size_t numStimuli);

	void addStimuliNode(Stimuli newStimuli);

	const std::vector<Stimuli>& getStimuli() const { return stimuli; }

private:
	size_t numStimuli;
	std::vector<Stimuli> stimuli;
};