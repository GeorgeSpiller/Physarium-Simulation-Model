#version 440 core

struct Agent
{
    float x, y;
    float angle;
};

layout(local_size_x = 64, local_size_y = 1) in;
layout(rgba32f) uniform image2D imgOutput;
layout(std430, binding = 2) buffer AgentBuffer
{
    Agent agents[];
} agent_buffer;

//uniform float deltaTime;

const ivec2 imgSize = imageSize(imgOutput);
const float agentMovmentSpeed = 180.0;

void main()
{
    uint i = gl_GlobalInvocationID.x;

    Agent currAgent = agent_buffer.agents[i];

    vec2 position = vec2(currAgent.x, currAgent.y);
    vec2 direction = vec2(cos(currAgent.angle), sin(currAgent.angle));
    vec4 agentColor = vec4(0.2, 0.6, 0.4, 1.0);

    position += direction * 180.0; // *deltaTime;

    agent_buffer.agents[i].x = position.x;
    agent_buffer.agents[i].y = position.y;

    barrier();
    imageStore(imgOutput, ivec2(position), agentColor);
}