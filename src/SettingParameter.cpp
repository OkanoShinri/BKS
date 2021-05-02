#include "SettingParameter.h"

SettingParameter::SettingParameter() //:
	//is_paddlemode(false), is_windowmode(true), bgm_volume(1.0), se_volume(1.0), myship_life(3), window_width(ofGetWidth()), window_height(ofGetHeight())
{
	is_paddlemode = false;
	is_windowmode = true;
	bgm_volume = 0.8;
	se_volume = 0.8;
	myship_life = 3;
	window_width = ofGetWidth();
	window_height = ofGetHeight();
}

void SettingParameter::set(bool _is_paddle_mode_on, bool _is_window_mode_on, float _bgm_volume, float _se_volume, int _myship_life)
{
}
