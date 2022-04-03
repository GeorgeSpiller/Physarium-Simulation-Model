#include <glad/glad.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include "LoadPrePattern.h"


LoadPrePattern::LoadPrePattern(unsigned int* tex_ID) : tex_ID(*tex_ID)
{
	prepatternDataRaw = NULL;
	prepatternData = NULL;
	imageWidth = NULL;
	imageHeight = NULL;
	numberOfChannels = 4; // always RGBA
}

LoadPrePattern::LoadPrePattern(const char* filePath, unsigned int* tex_ID) : tex_ID(*tex_ID)
{
	prepatternDataRaw = stbi_load(filePath, &imageWidth, &imageHeight, &numberOfChannels, 4);
	prepatternData = getUnsignedToFloats();
}


void LoadPrePattern::initalizeTexture(int windowWidth, int windowHeight) 
{
	glGenTextures(1, &tex_ID);
	glBindTexture(GL_TEXTURE_2D, tex_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	if (prepatternData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, prepatternData);
		glBindImageTexture(2, tex_ID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		free(prepatternData);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
		glBindImageTexture(1, tex_ID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		
	}
}

float* LoadPrePattern::getDataArray() 
{
	return prepatternData;
}

// private

float* LoadPrePattern::getUnsignedToFloats()
{

	/* From the stb_image.h docs:
	 The return value from an image loader is an 'unsigned char *' which points
	 to the pixel data, or NULL on an allocation failure or if the image is
	 corrupt or invalid. The pixel data consists of *y scanlines of *x pixels,
	 with each pixel consisting of N interleaved 8-bit components; the first		// (N 8-bit components = N floats)
	 pixel pointed to is top-left-most in the image. There is no padding between
	 image scanlines or between pixels, regardless of format. The number of
	 components N is 'desired_channels' if desired_channels is non-zero, or
	 *channels_in_file otherwise. If desired_channels is non-zero,
	 *channels_in_file has the number of components that _would_ have been
	 output otherwise. E.g. if you set desired_channels to 4, you will always
	 get RGBA output, but you can check *channels_in_file to see if it's trivially
	 opaque because e.g. there were only 3 channels in the source image.
	*/
	// The pixel data consists of imageHeight scanlines of imageWidth pixels
	// each pixel consists of 4 interleaved 8-bit components

	int pixNum = imageWidth * imageHeight; // int pixNum = imageWidth * imageHeight * numberOfChannels;
	float* dataArray;

	std::cout << "PrePattern image is " << pixNum << " pixels, " << pixNum * numberOfChannels << " color values total. " <<
		"It has a width x height of " << imageWidth << "x" << imageHeight << std::endl;

	dataArray = (float*)malloc(((pixNum * numberOfChannels) + 1) * sizeof(float));
	if (dataArray == NULL) exit(1);

	int pixelLocation = 0;
	int flipVandH = 0;

	int scanLineLength = (imageWidth * numberOfChannels);
	int scanLine = 0;
	int pixOffset = 0;
	float data[4] = {0, 0, 0, 0};

	/*
		Read data (unsigned char*) from prepatternDataRaw, into a float array. 
		During this reading process the image is flipped vertically and horrisontally.
	*/
	for (int y = 0; y < imageHeight; y++)
	{
		for (int x = 0; x < imageWidth; x++)
		{
			// the index of the curent scan line that is being read
			scanLine = (y * scanLineLength);
			// the pixel index offset of how far along that scan line we are reading
			pixOffset = (x * numberOfChannels);
			// the location of the current pixel we are reading
			pixelLocation = scanLine + pixOffset;

			// for each individual color channel (we do not want to flip these values)
			for (int c = 0; c < numberOfChannels; c++)
			{
				/*
				Read each pixel from the end of the scan line to the begining (but read each channel value for
				said pixel in-order). This flips the image horrisontally.
				We also normalise the (int) values and cast them to floats.
				*/
				data[c] = (float)prepatternDataRaw[((y+1) * scanLineLength) - pixOffset + c] / 255;
				/*
				Write the (now horrisontally flipped) read pixel value to the final float array,
				but write each scanline from the end of the array towards start. This flips the image virtically. 
				*/
				dataArray[((pixNum * numberOfChannels) - pixelLocation - numberOfChannels) + c] = data[c];
			}
		}
	}
	return dataArray;
}
