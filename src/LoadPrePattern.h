#include "stb_image.h"

class LoadPrePattern 
{
public:
	LoadPrePattern(char * filePath, unsigned int *tex_ID);
	~LoadPrePattern() { 
		stbi_image_free(prepatternDataRaw); 
		delete prepatternDataFloatPointer;
	
	}; // assertion because of the free

	void initalizeTexture();
	float* getUnsignedToFloats();
	void deallocate();

	int getWidth() { return imageWidth; };
	int getHeight() { return imageHeight; };
	unsigned char* getDataRaw() { return prepatternDataRaw; };

private:

	// https://stackoverflow.com/questions/825344/casting-an-array-of-unsigned-chars-to-an-array-of-floats
	template <typename T> // T models Any
	struct static_cast_func
	{
		template <typename T1> // T1 models type statically convertible to T
		T operator()(const T1& x) const { return static_cast<T>(x); }
	};

	void initTexture();
	void lpp_RGB_UnsignedByte();
	void lpp_RGBA_UnsignedByte();

	int imageWidth;
	int imageHeight;
	int numberOfChannels;
	unsigned char* prepatternDataRaw;
	float* prepatternDataFloatPointer;
	unsigned int tex_ID;
};