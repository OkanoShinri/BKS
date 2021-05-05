#pragma once

#include "ofMain.h"
#include <vector>
#include <memory>

class BackGroundImage
{
private:
	std::vector<std::unique_ptr<ofImage>> images;
public:
	BackGroundImage();
	void updata();
	void draw();
	void changeImage();
};
