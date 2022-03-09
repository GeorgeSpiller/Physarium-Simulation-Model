
#version 450 core
layout(location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout(location = 1) in vec3 aColor; // the color variable has attribute position 1

out vec3 ourColor; // output a color to the fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.5);
    ourColor = aColor; // set ourColor to the input color we got from the vertex data
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