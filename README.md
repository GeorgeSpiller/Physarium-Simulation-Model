
# Physarum Polycephalum Simulation

This project renders a simulation of the slime mold Physarum polycephalum.


## Deployment

To deploy this project run

```bash
  Application.cpp
```


## Features

- Fully tunable parameters
- Six spawn modes:
    - RANDOM, CIRCLE, POINT, POINT2, POINT4, RECT
- Agent movement and deposition processed in parallel on the GPU

### Parameter Explanation:
**WINDOW_IS_FULLSCREEN**\
Not fully implemented. If false, WINDOW_WIDTH and WINDOW_HEIGHT are used to size the window. If true, the window size will be 1920 x 1080.\
**WINDOW_WIDTH** \
If WINDOW_IS_FULLSCREEN is set, this value is ignored. This sets the window width of the simulation. \
**WINDOW_HEIGHT** \
If WINDOW_IS_FULLSCREEN is set, this value is ignored. This sets the window height of the simulation. \
**TRAILMAP_trailDiffuseSpeed** \
The diffusion speed of the trail that each  agent deposits onto the trail map. A higher value = shorter trail. \
**RAILMAP_trailEvaporationSpeed** \
The evaporation speed of the trail map. A higher value = trails evaporate faster. \
**AGENT_movmentSpeed** \
This is how fast the agents move each frame (NOT how fast the simulation runs, this simulation will always run as fast as possible). \
**AGENT_turnSpeed** \
The turn speed of each agent in degrees. \
**AGENT_sensorOffsetDst** \
How far away the sensors (F) are from each agent. \
**AGENT_sensorAngleSpacing** \
The angle difference in degrees of the FL and FR sensor from the F sensor of each agent. \
**AGENT_sensorSize** \
The size of the sensor sampling area (always square). WARNING: changing this can greatly decrease performance, but will allow each agent to 'see' more of their environment, and therefore tends to improve following behavior. \
**NUMBER_OF_AGENTS** \
The number of agents to spawn in the simulation. This should always be multiples of 64: 50048, 60032, 70016, 80000, 90048, 100032, ..., 499008.


## Roadmap
- Code clean up, resolving memory leaks
- Removing hardcoded file paths
- Reading model parameters from a json
- Removing bounds on the model parameters (dynamic workgroup sizes)
- full screen mode
- Additional pre-pattern image conversion:
    - support for images with no alpha channel
    - support for images of different pixel color value storage sizes and types.




