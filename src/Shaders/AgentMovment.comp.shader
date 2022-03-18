#version 440 core
#define FLT_MAX 3.402823466e+38
#define TWO_PI  6.2831853
#define PI      3.1415926
#define HALF_PI 1.5707963

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
};

// these uniforms change each frame, are the same for all agents
uniform float deltaTime;
uniform float randomSeed;
// these uniforms are constant, are the same for all agents
uniform float agentMovmentSpeed;
uniform vec3 agentSensorSettings;
uniform float agentTurnSpeed;

const ivec2 imgSize = imageSize(imgOutput);
float sensorOffsetDst = agentSensorSettings.x;
float sensorAngleSpacing = agentSensorSettings.y;
int sensorSize = int(agentSensorSettings.z);

// https://www.shadertoy.com/view/XlXcW4
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


vec2 outOfBounds_WrapArround(vec2 pos) 
{
    if (pos.x < 0)
    {
        pos.x = imgSize.x;
    }
    else if (pos.x > imgSize.x)
    {
        pos.x = 0;
    }
    else if (pos.y < 0)
    {
        pos.y = imgSize.y;
    }
    else if (pos.y > imgSize.y)
    {
        pos.y = 0;
    }
    return pos;
}

float randomHalfAngle(vec2 pos, uint AgentIndex, uint rand)
{
    float returnAngle = 0.0;

    pos.x = min(imgSize.x - 0.01, max(0, pos.x));
    pos.y = min(imgSize.y - 0.01, max(0, pos.y));
    returnAngle = scaleRantomTo01(rand) * (TWO_PI / 2);
    returnAngle += HALF_PI / 10;
    if (returnAngle > PI) { returnAngle -= HALF_PI / 5; }
    return returnAngle;
}

float sense(Agent agent, float sensorAngleOffset, vec4 agentColor)
{
    float sensorAngle = agent.angle + sensorAngleOffset;
    vec2 sensorDir = angleToVector(sensorAngle); // vec2(cos(sensorAngle), sin(sensorAngle));

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
    float randHalfAngleVal = randomHalfAngle(position, i, random);
    bool isOOB = false;

    // OOB check
    //if (position.x < 0)
    //{
    //    position.x = 0;
    //    randHalfAngleVal += PI + HALF_PI;
    //    isOOB = true;
    //}
    //else if (position.x > imgSize.x)
    //{
    //    position.x = imgSize.x;
    //    randHalfAngleVal += HALF_PI;
    //    isOOB = true;
    //}
    //else if (position.y < 0)
    //{
    //    position.y = 0;
    //    isOOB = true;
    //}
    //else if (position.y > imgSize.y)
    //{
    //    position.y = imgSize.y;
    //    randHalfAngleVal += PI;
    //    isOOB = true;
    //}
    //
    //if (isOOB) 
    //{
    //    if (randHalfAngleVal > TWO_PI) { randHalfAngleVal -= TWO_PI; }
    //    agents[i].angle = randHalfAngleVal;
    //}

    vec2 direction = angleToVector(currAgent_read.angle); // vec2(cos(currAgent_read.angle), sin(currAgent_read.angle));
    vec4 agentColor = vec4(0.2, 0.6, 0.4, 1.0);

    position += direction * agentMovmentSpeed * deltaTime;

    position = outOfBounds_WrapArround(position);

    float weightForward = sense(currAgent_read, 0, agentColor);
    float weightLeft = sense(currAgent_read, sensorAngleSpacing, agentColor);
    float weightRight = sense(currAgent_read, -sensorAngleSpacing, agentColor);

    float randomSteerStrength = scaleRantomTo01(random) / 2;

    //if (weightForward > weightLeft && weightForward > weightRight) 
    //else if (weightForward < weightLeft && weightForward < weightRight)
    //    agents[i].angle += (randomSteerStrength - 0.5) * 2 * agentTurnSpeed * deltaTime;
    //else if (weightRight > weightLeft)
    //    agents[i].angle -= randomSteerStrength * agentTurnSpeed * deltaTime;
    //else if (weightLeft > weightRight)
    //    agents[i].angle += randomSteerStrength * agentTurnSpeed * deltaTime;


    if (weightForward > weightLeft && weightForward > weightRight) 
    {
        // stay facing the same direction
    }
    else if (weightForward < weightLeft && weightForward < weightRight)
    {   /// rotate randomly left or right
        //agents[i].angle += (randomSteerStrength - 0.5) * 2 * agentTurnSpeed * deltaTime;
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

/*
*     if (position.x < 100) 
    {
        if (agentAngle > PI && agentAngle < PI + HALF_PI)
        {   // if approaching from above the surface normal
            agents[i].angle = calculateIncedenceOnWestWall(agentAngle, false);
        }
        else if (agentAngle > HALF_PI && agentAngle < PI)
        {
            agents[i].angle = calculateIncedenceOnWestWall(agentAngle, true);
        }
    }
    else if (position.x > imgSize.x - 100)
    {
        if (agentAngle > HALF_PI && agentAngle < PI)
        {   // if approaching from above the surface normal
            AgentAngleOffset = PI + (TWO_PI - agentAngle);
            agents[i].angle = calculateIncedenceOnWestWall(agentAngle, false);
        }
        else if (agentAngle > PI && agentAngle < PI + HALF_PI)
        {
            agents[i].angle = calculateIncedenceOnWestWall(agentAngle, true);
        }
    }
* 
* 
float calculateIncedenceOnWestWall(float agentAngle, bool isFromBot)
{
    float incedentAngle = 0.0;
    float tmp = 0.0;

    if (!isFromBot)
    {
        // if approaching from above the surface normal
        tmp = (PI + HALF_PI) - agentAngle;
        incedentAngle = PI - (tmp + HALF_PI);
        return (PI + HALF_PI) + incedentAngle;
    }
    else
    {   // if approaching from bellow the sureface normal
        tmp = agentAngle - HALF_PI;
        incedentAngle = PI - (tmp + HALF_PI);
        return incedentAngle;
    }
}
*/
