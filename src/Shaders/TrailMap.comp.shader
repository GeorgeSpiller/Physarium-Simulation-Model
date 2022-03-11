#version 440 core

layout(local_size_x = 8, local_size_y = 8) in;
layout(rgba32f) uniform readonly image2D imgInput;
layout(rgba32f) uniform writeonly image2D imgOutput;

uniform float deltaTime;

void main()
{
    ivec2 imgSize = imageSize(imgInput);
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
    vec4 initValue = imageLoad(imgInput, pixelCoords);

    imageStore(imgOutput, pixelCoords, initValue);
}