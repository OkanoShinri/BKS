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
	int num_ball = 3;
	std::string version = "0.93";
};

/*
class SettingParameter
{
private:
	bool is_paddlemode = false;
	bool is_windowmode = true;
	float bgm_volume = 0.8;
	float se_volume = 0.8;
	int myship_life = 3;
	int window_width = ofGetWidth();
	int window_height = ofGetHeight();
public:
	SettingParameter();
	~SettingParameter() {
		std::cout << "Remove: SettingParameter" << std::endl;
	};
	void set(bool _is_paddle_mode_on, bool _is_window_mode_on, float _bgm_volume, float _se_volume, int _myship_life);
	void setBGMVolume(float _bgm_volume) {
		bgm_volume = _bgm_volume;
	}
	void setSEVolume(float _se_volume) {
		se_volume = _se_volume;
	}
	float getWidth() {
		return window_width;
	}
	float getHeight() {
		return window_height;
	}
	float getBGMVolume() {
		return bgm_volume;
	}
	float getSEVolume() {
		return se_volume;
	}
	bool isWindowMode() {
		return is_windowmode;
	}
};
*/
