#version 440 core

layout(local_size_x = 8, local_size_y = 8) in;
layout(rgba32f) uniform readonly image2D imgInput;
layout(rgba32f) uniform writeonly image2D imgOutput;

uniform float deltaTime;
uniform float trailDiffuseSpeed;
uniform float trailEvaporationSpeed;

void main()
{
    ivec2 imgSize = imageSize(imgInput);
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
    vec4 originalValue = imageLoad(imgInput, pixelCoords);
    
    // vec4 outputValue = vec4(originalValue.r - trailDiffuse, originalValue.g - trailDiffuse, originalValue.b - trailDiffuse, 1.0);
   /* outputValue.r = max(outputValue.r, 0);
    outputValue.g = max(outputValue.g, 0);
    outputValue.b = max(outputValue.b, 0);*/

    // 3x3 mean kernel
    vec4 sum = vec4(0.0);
    ivec2 currCoord = ivec2(0);
    int sampleAmount = 0;
    for (int j = -1; j <= 1; j++) 
    {
        for (int k = -1; k <= 1; k++) 
        {
            currCoord = ivec2(pixelCoords.x + j, pixelCoords.y + k);
            // bonuds checking
            if (currCoord.x > 0 && currCoord.x < imgSize.x)
            {
                if (currCoord.y > 0 && currCoord.y < imgSize.y)
                {
                    sum += imageLoad(imgInput, currCoord);
                    sampleAmount++;
                }
            }
        }
    }
    vec4 meanKernel = sum / sampleAmount;

    // Linear interpolation(x, y) weight = a : x * (1 - a) + y * a;
    vec4 diffuse = originalValue * (1 - (trailDiffuseSpeed * deltaTime)) + meanKernel * (trailDiffuseSpeed * deltaTime);
    vec4 diffEvap = diffuse - (trailEvaporationSpeed * deltaTime);
    vec4 outputValue = vec4(diffEvap.r, diffEvap.g, diffEvap.b, 1.0);

    imageStore(imgOutput, pixelCoords, outputValue);
}