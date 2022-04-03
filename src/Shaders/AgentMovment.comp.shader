#version 440 core
#define PI      3.1415926
#define HALF_PI 1.5707963

struct Agent
{
    float x, y;
    float angle;
};

layout(local_size_x = 64, local_size_y = 1) in;
layout(rgba32f, binding = 1) uniform image2D imgOutput;
layout(std430, binding = 2) buffer AgentBuffer
{
    Agent agents[];
};

// these uniforms change each frame, are the same for all agents
uniform float deltaTime;
// these uniforms are constant, are the same for all agents
uniform float agentMovmentSpeed;
uniform vec3 agentSensorSettings;
uniform float agentTurnSpeed;

const ivec2 imgSize = imageSize(imgOutput);
float sensorOffsetDst = agentSensorSettings.x;
float sensorAngleSpacing = agentSensorSettings.y;
int sensorSize = int(agentSensorSettings.z);

uint hash(uint state) 
{
    state ^= 2747636419;
    state *= 2654435769;
    state ^= state >> 16;
    state *= 2654435769;
    state ^= state >> 16;
    state *= 2654435769;
    return state;
}

float scaleRantomTo01(uint random)
{
    return float(random) / 4294967295.0;
}

vec2 angleToVector(float angle) 
{
    return vec2(cos(angle), sin(angle));
}

float randomHalfAngle(uint rand)
{
    return scaleRantomTo01(rand) * (PI / 2);
}

float sense(Agent agent, float sensorAngleOffset, vec4 agentColor)
{
    float sensorAngle = agent.angle + sensorAngleOffset;
    vec2 sensorDir = angleToVector(sensorAngle);

    ivec2 sensorCenter = ivec2(vec2(agent.x, agent.y) + sensorDir * sensorOffsetDst);
    float sum = 0.0;

    for (int x_off = -sensorSize; x_off <= sensorSize; x_off++)
        for (int y_off = -sensorSize; y_off <= sensorSize; y_off++)
        {
            ivec2 pos = sensorCenter + ivec2(x_off, y_off);
            if (pos.x >= 0 && pos.x < imgSize.x && pos.y >= 0 && pos.y < imgSize.y)
                sum += dot(imageLoad(imgOutput, pos), agentColor * 2 - 1);
        }
    return sum;
}


void main()
{
    uint i = gl_GlobalInvocationID.x;
    Agent currAgent_read = agents[i];
    vec2 position = vec2(currAgent_read.x, currAgent_read.y);
    uint random = hash(uint(position.y * imgSize.y + position.x + hash(i)));
    float randHalfAngleVal = randomHalfAngle(random);
    float OOB_angle = currAgent_read.angle;

    // out of bounds, iether random angle or wrap around. Random angle can lead to agents hugging boarders
    if (position.x < 0)
    {
        OOB_angle = randHalfAngleVal + (PI + HALF_PI);
        // agents[i].position.x = imgSize.x;
    }
    else if (position.x > imgSize.x)
    {
        OOB_angle = randHalfAngleVal + HALF_PI;
        // agents[i].position.x = 0;
    }
    else if (position.y < 0)
    {
        OOB_angle = randHalfAngleVal;
        // agents[i].position.y = imgSize.y;
    }
    else if (position.y > imgSize.y)
    {
        OOB_angle = randHalfAngleVal + PI;
        // agents[i].position.y = 0;
    }
    if (OOB_angle > PI * 2)
    {
        OOB_angle -= (PI * 2);
    }
    agents[i].angle = OOB_angle;

    vec2 direction = angleToVector(currAgent_read.angle);
    vec4 agentColor = vec4(0.2, 0.6, 0.4, 1.0); // un-normalised 255 RGB: (51, 153, 102)

    position += direction * agentMovmentSpeed * deltaTime;

    float weightForward = sense(currAgent_read, 0, agentColor);
    float weightLeft = sense(currAgent_read, sensorAngleSpacing, agentColor);
    float weightRight = sense(currAgent_read, -sensorAngleSpacing, agentColor);

    float randomSteerStrength = scaleRantomTo01(random) / 2;

    if (weightForward > weightLeft && weightForward > weightRight) 
    {
        // stay facing the same direction
    }
    else if (weightForward < weightLeft && weightForward < weightRight)
    {   /// rotate randomly left or right
        if (randomSteerStrength > 0.5)
        {
            agents[i].angle -= randomSteerStrength * agentTurnSpeed * deltaTime;
        }
        else
        {
            agents[i].angle += randomSteerStrength * agentTurnSpeed * deltaTime;
        }
    }
    else if (weightRight > weightLeft) 
    {
        agents[i].angle -= randomSteerStrength * agentTurnSpeed * deltaTime;
    }
    else if (weightLeft > weightRight) 
    {
        agents[i].angle += randomSteerStrength * agentTurnSpeed * deltaTime;
    }
    
    agents[i].x = position.x;
    agents[i].y = position.y;

    imageStore(imgOutput, ivec2(position), agentColor);
}
