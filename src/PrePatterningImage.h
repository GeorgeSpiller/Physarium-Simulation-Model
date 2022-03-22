class PrePatterningImage
{
public:
	PrePatterningImage();
	~PrePatterningImage();

	unsigned char* getImageData() { return imageData; };
	int getWidth() { return imageWidth; };
	int getHeight() { return imageHeight; };

private:
	int imageWidth;
	int imageHeight;
	int nrChannels;
	unsigned char* imageData;
};