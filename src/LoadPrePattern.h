#include "stb_image.h"

class LoadPrePattern 
{
public:
	unsigned int tex_ID;

	LoadPrePattern(unsigned int* tex_ID);
	LoadPrePattern(const char * filePath, unsigned int *tex_ID);
	

	~LoadPrePattern() { 
		stbi_image_free(prepatternDataRaw); 	
	};

	int getWidth() { return imageWidth; };
	int getHeight() { return imageHeight; };
	unsigned char* getDataRaw() { return prepatternDataRaw; };
	float* getDataArray();
	void initalizeTexture(int winowWidth, int windowHeight);

private:

	float* getUnsignedToFloats();
	
	int imageWidth;
	int imageHeight;
	int numberOfChannels;
	unsigned char* prepatternDataRaw;
	float* prepatternData;	
};