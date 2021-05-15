#include "BackGroundImage.h"

BackGroundImage::BackGroundImage()
{
	images.emplace_back(std::make_unique<ofImage>());
	images.back()->load("nc229388.jpg");
}

void BackGroundImage::updata()
{
}

void BackGroundImage::draw()
{
	//images.back()->draw(0, 0);
	ofSetColor(255, 255, 255);
	ofDrawRectangle(0, 0, 1280, 720);
}

void BackGroundImage::changeImage()
{
}
