#pragma once
#include "ofMain.h"
class SettingParameter
{
private:
	bool is_paddlemode;
	bool is_windowmode;
	float bgm_volume;
	float se_volume;
	int myship_life, window_width, window_height;
public:
	SettingParameter();
	~SettingParameter() {
		std::cout << "Remove: SettingParameter" << std::endl;
	};
	void set(bool _is_paddle_mode_on, bool _is_window_mode_on, float _bgm_volume, float _se_volume, int _myship_life);
	void setBGMVolume(float _bgm_volume) {
		this->bgm_volume = _bgm_volume;
	}
	float getWidth() {
		return this->window_width;
	}
	float getHeight() {
		return this->window_height;
	}
	float getBGMVolume() {
		return this->bgm_volume;
	}
	bool isWindowMode() {
		return this->is_windowmode;
	}
};