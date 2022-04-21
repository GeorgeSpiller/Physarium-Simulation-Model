#version 440 core
#define TWO_PI  6.2831853
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
uniform vec3 agentColor;
const float sensorOffsetDst = agentSensorSettings.x;
const float sensorAngleSpacing = agentSensorSettings.y;
const int sensorSize = int(agentSensorSettings.z);
const ivec2 imgSize = imageSize(imgOutput);

/*
    Hash function, used when generating random integers.
    Based on state (seed) it will return a hash of size uint
*/
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

/*
    Normalizes an int
*/
float scaleRantomTo01(uint random)
{
    return float(random) / 4294967295.0;
}

/* https://math.stackexchange.com/questions/180874/convert-angle-radians-to-a-heading-vector
    Firstly, format the angle so that its within the range [0, TWO_PI].
    Finally, return the vec2 normalised heading of this angle.
*/
vec2 angleToVector(float angle) 
{
    if (angle > TWO_PI)
    {
        angle -= TWO_PI;
    }
    if (angle < 0) 
    {
        angle += TWO_PI;
    }
    return vec2(cos(angle), sin(angle));
}

/*
    Handels agent sencing.
    Firstly, calculate the 2vec position of the sensor in the image enviroment.
    Finally, return the sum of the RGB values read at the sensor coordinate.
*/
float sense(Agent agent, float sensorAngleOffset, vec4 agentCol)
{
    float sensorAngle = agent.angle + sensorAngleOffset;
    float sum = 0.0;
    vec2 sensorDir = angleToVector(sensorAngle);
    ivec2 sensorCenter = ivec2(vec2(agent.x, agent.y) + sensorDir * sensorOffsetDst);
    vec4 sencedValue = imageLoad(imgOutput, sensorCenter);

    if (sensorSize <= 1)
    {
        if (sensorCenter.x >= 0 && sensorCenter.x < imgSize.x && sensorCenter.y >= 0 && sensorCenter.y < imgSize.y) 
        {
            return sencedValue.r + sencedValue.g + sencedValue.b;
            //return dot(imageLoad(imgOutput, sensorCenter), agentCol);
        }  
    }

    for (int x_off = -sensorSize; x_off <= sensorSize; x_off++)
    {
        for (int y_off = -sensorSize; y_off <= sensorSize; y_off++)
        {
            ivec2 pos = sensorCenter + ivec2(x_off, y_off);
            if (pos.x >= 0 && pos.x <= imgSize.x)
            {
                if (pos.y >= 0 && pos.y <= imgSize.y)
                {   // if pos is in bounds
                    sencedValue = imageLoad(imgOutput, pos);
                    sum += sencedValue.r + sencedValue.g + sencedValue.b;
                } else // x in bounds, y out of bounds
                {
                    if (pos.y > imgSize.y)
                    {
                        pos.y -= imageSize.y;
                        sencedValue = imageLoad(imgOutput, pos);
                        sum += sencedValue.r + sencedValue.g + sencedValue.b;
                    }
                    else if (pos.y < 0) 
                    {
                        pos.y += imageSize.y;
                        sencedValue = imageLoad(imgOutput, pos);
                        sum += sencedValue.r + sencedValue.g + sencedValue.b;
                    }
                }
            } else  // x out of bounds, y in bounds
            {
                if (pos.x > imgSize.x)
                {
                    pos.x -= imageSize.x;
                    sencedValue = imageLoad(imgOutput, pos);
                    sum += sencedValue.r + sencedValue.g + sencedValue.b;
                }
                else if (pos.x < 0) 
                {
                    pos.x += imageSize.x;
                    sencedValue = imageLoad(imgOutput, pos);
                    sum += sencedValue.r + sencedValue.g + sencedValue.b;
                }

            }
        }
    }
    return sum;
}


void main()
{
    uint i = gl_GlobalInvocationID.x;
    Agent currAgent_read = agents[i];
    vec2 position = vec2(currAgent_read.x, currAgent_read.y);
    vec2 direction = angleToVector(currAgent_read.angle);
    vec4 currAgent_color = vec4(agentColor.r, agentColor.g, agentColor.b, 1.0); 
    uint random = hash(uint(position.y * imgSize.y + position.x + hash(i)));
    bool hasMoved = false;

    // [Motor Stage] 
    vec4 nextPosValue = imageLoad(imgOutput, ivec2(position + direction * agentMovmentSpeed));

    if (nextPosValue == currAgent_color)
    {
        currAgent_read.angle = scaleRantomTo01(random) * TWO_PI;
        agents[i].angle = currAgent_read.angle;
        direction = angleToVector(currAgent_read.angle);
        hasMoved = false;
    }
    else 
    {
        position += direction * agentMovmentSpeed; // *deltaTime;
        hasMoved = true;
    }

    if (position.x < 0)
    {
        position.x += imgSize.x;
    }
    else if (position.x > imgSize.x)
    {
        position.x -= imgSize.x;
    }
    else if (position.y < 0)
    {
        position.y += imgSize.y;
    }
    else if (position.y > imgSize.y)
    {
        position.y -= imgSize.y;
    }

    agents[i].x = position.x;
    agents[i].y = position.y;

    // [Sensory Stage]
    float weightForward = sense(currAgent_read, 0.0, currAgent_color);
    float weightLeft = sense(currAgent_read, sensorAngleSpacing, currAgent_color);
    float weightRight = sense(currAgent_read, -sensorAngleSpacing, currAgent_color);

    if (weightForward > weightLeft && weightForward > weightRight) 
    {
        // stay facing the same direction
    }
    else if (weightForward < weightLeft && weightForward < weightRight)
    {   /// rotate randomly left or right
        if (scaleRantomTo01(random) > 0.5)
        {
            agents[i].angle -= agentTurnSpeed;
        }
        else
        {
            agents[i].angle += agentTurnSpeed;
        }
    }
    else if (weightLeft < weightRight) 
    {
        agents[i].angle -= agentTurnSpeed;
    }
    else if (weightRight < weightLeft) 
    {
        agents[i].angle += agentTurnSpeed;
    }
    else 
    {
        // stay facing the same direction
    }

    if (hasMoved)
    {   // only deposit if the agent was able to move forward
        imageStore(imgOutput, ivec2(position), currAgent_color);
    }
    
}
