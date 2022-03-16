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

uniform float deltaTime;
uniform float agentMovmentSpeed;
uniform float debug_newAngle;
uniform float randomSeed;

const ivec2 imgSize = imageSize(imgOutput);
const uint k = 1103515245U;  // GLIB C, used for randomness

// https://www.shadertoy.com/view/XlXcW4
vec3 hash(uvec3 x)
{
    x = ((x >> 8U) ^ x.yzx) * k;
    x = ((x >> 8U) ^ x.yzx) * k;
    x = ((x >> 8U) ^ x.yzx) * k;
    vec3 vout = vec3(x) * (1.0 / float(0xffffffffU));
    return vout; //(vout.x + vout.y + vout.z) / 3
}

//float randomHalfAngle(uint seed)
//{
//    float psudoRandomFloat = float(hash(uvec3(seed, randomSeed, imgSize.y)));
//    return (psudoRandomFloat / 4294967295.0) * TWO_PI;
//}

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

float outOfBounds_RandomAngle(vec2 pos, float agentAngle)
{
    //float randHalfAng = randomHalfAngle(uint(pos.x + pos.y + agentAngle));
    //float outputAngle = agentAngle;
    //if (pos.x < 100)
    //{
    //    outputAngle = randHalfAng + (PI + HALF_PI);
    //}
    // 
    //else if (pos.x > imgSize.x)
    //{
    //    outputAngle = randomHalfAngle + (PI / 2);
    //}
    //else if (pos.y < 0)
    //{
    //    outputAngle = randomHalfAngle;
    //}
    //else if (pos.y > imgSize.y)
    //{
    //    outputAngle = randomHalfAngle + PI;
    //}
    if (outputAngle > TWO_PI) { outputAngle -= TWO_PI; }
    return outputAngle;
}


void main()
{
    uint i = gl_GlobalInvocationID.x;

    Agent currAgent_read = agents[i];
    vec2 position = vec2(currAgent_read.x, currAgent_read.y);

    agents[i].angle = outOfBounds_RandomAngle(position, agents[i].angle);

    vec2 direction = vec2(cos(currAgent_read.angle), sin(currAgent_read.angle));
    vec4 agentColor = vec4(0.2, 0.6, 0.4, 1.0);

    position = hash(uvec3(1, 1, 1))

    position += direction * agentMovmentSpeed * deltaTime;

    // position = outOfBounds_WrapArround(position);

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
