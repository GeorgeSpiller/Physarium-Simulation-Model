#version 440 core

layout(location = 0) in vec2 uv;
layout(location = 1) in vec2 tex;
out vec2 coords;

void main()
{
    coords = tex;
    gl_Position = vec4(uv, 0.0, 1.0);
}


// vertex shader when using texture
//#version 450 core
//
//// https://learnopengl.com/Getting-started/Textures
//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec2 aTexCord;
//out vec3 ourColor;
//out vec2 TextCord;
//
//void main()
//{
//	// simply forward in's to frag shader
//	gl_Position = vec4(aPos, 1.0);
//	ourColor = aColor;
//	TexCoord = aTexCoord;
//}