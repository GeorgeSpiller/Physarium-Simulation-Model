#version 450 core

layout(local_size_x = 8, local_size_y = 8) in;
layout(rgba32f) uniform readonly image2D inpImg;
layout(rgba32f) uniform writeonly image2D outImg;

uniform float deltaTime;

void main()
{
    ivec2 imgSize = imageSize(inpImg);
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
    vec4 initValue = imageLoad(inpImg, pixelCoords);

    imageStore(outImg, pixelCoords, initValue);
}