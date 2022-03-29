#include "stb_image.h"

class LoadPrePattern 
{
public:
	LoadPrePattern(char * filePath, unsigned int *tex_ID);
	~LoadPrePattern() { 
		stbi_image_free(prepatternDataRaw); 	
	}; // assertion because of the free

	void initalizeTexture();
	float* getUnsignedToFloats();

	int getWidth() { return imageWidth; };
	int getHeight() { return imageHeight; };
	unsigned char* getDataRaw() { return prepatternDataRaw; };

private:
	

	void initTexture();
	
	int imageWidth;
	int imageHeight;
	int numberOfChannels;
	unsigned char* prepatternDataRaw;
	unsigned int tex_ID;
};