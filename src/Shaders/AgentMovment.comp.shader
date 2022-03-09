#version 450 core

struct Agent
{
    float x, y;
    float angle;
};

layout(local_size_x = 64, local_size_y = 1) in;
layout(rgba32f) uniform image2D imgOutput;
layout(std430, binding = 2) buffer Agents
{
    Agent agents[];
};

uniform float deltaTime;

const ivec2 imgSize = imageSize(imgOutput);

void main()
{
    uint i = gl_GlobalInvocationID.x;
    Agent currAgent = agents[i];
    vec2 position = vec2(currAgent.x, currAgent.y);
    vec4 agentColor = vec4(0.2, 0.6, 0.4, 1.0);

    position += 180.0 * deltaTime;

    imageStore(imgOutput, ivec2(position), agentColor);
}