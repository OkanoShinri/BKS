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
	images.back()->draw(0, 0);
	ofSetColor(255, 255, 255, 200);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
}

void BackGroundImage::changeImage()
{
}
