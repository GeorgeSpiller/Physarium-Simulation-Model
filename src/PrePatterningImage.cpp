#include <iostream>
#include "PrePatterningImage.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

PrePatterningImage::PrePatterningImage()
{
	// imageData = stbi_load("D:\\Users\\geosp\\Documents\\__Work\\.Uni\\FinalYear\\Diss\\PhysarumSimulation\\PhysarumSimulation\\src\\prepat.png", &imageWidth, &imageHeight, &nrChannels, 0);
	std::cout << "w: " << imageWidth << ", h: " << imageHeight << ", nrCh: " << nrChannels << std::endl;
	// std::cout << "stbi Error stack: " << stbi_failure_reason() << std::endl;
}

PrePatterningImage::~PrePatterningImage() 
{
	// stbi_image_free(imageData);
}