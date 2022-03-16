#version 440 core

in vec2 coords;

uniform sampler2D texture1;

out vec4 FragColor;

void main()
{
    FragColor = texture(texture1, coords);
}

//#version 440 core
//
//in vec2 coords;
//uniform sampler2D tex;
//out vec4 FragCol;
//
//void main()
//{
//    FragCol = texture(tex, coords);
//}


// fragment shader for when using texture
//#version 450 core
//
//out vec4 FragColor;
//in vec3 ourColor;
//in vec2 TexCoord;
//
//uniform sampler2D ourTexture;
//
//void main()
//{
//
//	FragColor = texture(ourTexture, TexCoord);
//
//	//FragColor = vec4(ourColor, 1.0f);
//	// if the current pix pos is within the bounds of a 3x3 kernal
//	//if (gl_FragCoord.x > 1.0 ) // && gl_FragCoord.x < 100.0
//	//{
//	//	vec4 convSum = vec4(0.0f, 0.0f, 0.0f, 1.0f);
//	//	for (int j = -1; i<3; i++;) 
//	//	{
//	//		for (int k = -1; k < 3; k++;)
//	//		{
//	//			convSum.x += gl_FragCoord.x;
//	//			convSum.y += gl_FragCoord.y;
//	//		}
//	//	}
//	//	FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
//	//	// FragColor = vec4(ourColor, 1.0f);
//	//} else 
//	//{
//	//	FragColor = vec4(ourColor, 1.0f);
//	//}
//	// sum up values from -1, 1 on the X and Y
//	// divide sum by 9, set current fragColor to value
//}