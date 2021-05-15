#pragma once
#include "ofMain.h"
struct SettingParameter
{
	bool is_paddlemode = false;
	bool is_fullscreen = false;
	float bgm_volume = 0.8;
	float offset_x = 0;
	float offset_y = 0;
	float scale = 1.0;
	float se_volume = 0.8;
	int myship_life = 3;
	int window_height = 720;
	int window_width = 1280;
	int num_ball = 1;
	int MAX_BALL = 10;
	std::string version = "0.96";
};
