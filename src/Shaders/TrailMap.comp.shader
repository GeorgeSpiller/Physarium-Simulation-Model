#version 440 core

layout(local_size_x = 8, local_size_y = 8) in;
layout(rgba32f, binding = 1) uniform readonly image2D imgInput;
layout(rgba32f, binding = 2) uniform readonly image2D imgPrePattern;
layout(rgba32f, binding = 1) uniform writeonly image2D imgOutput; 

uniform float deltaTime;
uniform float trailDiffuseSpeed;
uniform float trailEvaporationSpeed;
uniform vec3 agentColor;

void main()
{
    ivec2 imgSize = imageSize(imgInput);
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);

    // 3x3 mean kernel
    vec4 sum = vec4(0.0);
    vec4 prePatSample = vec4(0.0);
    vec4 outSample = vec4(0.0);

    ivec2 currCoord = ivec2(0);
    int sampleAmount = 0;
    for (int j = -1; j <= 1; j++) 
    {
        for (int k = -1; k <= 1; k++) 
        {
            currCoord = ivec2(pixelCoords.x + j, pixelCoords.y + k);
            // bonuds checking
            if (currCoord.x >= 0 && currCoord.x <= imgSize.x)
            {
                if (currCoord.y >= 0 && currCoord.y <= imgSize.y)
                {   // if currSample is in bounds, currCoord is correct
                    
                } else // x in, y out
                {
                    if (currCoord.y > imgSize.y) // if Y out of upper bound
                    {
                        currCoord.y -= imgSize.y;
                    } else if (currCoord.y < 0) // if y out of lower bound
                    {
                        currCoord.y += imgSize.y;
                    }
                }
            } 
            else // x out, y in
            {
                if (currCoord.x > imgSize.x) // if x out of upper bound
                {
                    currCoord.x -= imgSize.x;
                } 
                else if (currCoord.x < 0) // if x out of lower bound
                {
                    currCoord.x += imgSize.x;
                }
            }

            prePatSample = imageLoad(imgPrePattern, currCoord);
            outSample = imageLoad(imgInput, currCoord);

            if ((prePatSample.r + prePatSample.g + prePatSample.b) > (outSample.r + outSample.g + outSample.b))
            {
                sum += prePatSample;
            }
            else 
            {
                sum += outSample;
            }
            sampleAmount++;
        }
    }
    vec4 meanKernel = sum / sampleAmount;

    vec4 diffuse = meanKernel * trailDiffuseSpeed;
    vec4 outputValue = vec4(diffuse.r, diffuse.g, diffuse.b, 1.0);
    
    imageStore(imgOutput, pixelCoords, outputValue);
}