#include "Scene.h"

QuitScene::QuitScene(std::unique_ptr<SettingParameter>&& _setting_parameter)
{
	setting_parameter = std::move(_setting_parameter);
}

TitleScene::TitleScene(std::unique_ptr<SettingParameter>&& _setting_parameter)
{
	setting_parameter = std::move(_setting_parameter);
	transition_counter = 0;
	transition_time = 60;
	can_change_scene = false;
	is_transiting = false;
	next_scene = Scene::play_game_scene;

	choice_idx = 0;
	push_counter = 0;

	Oranienbaum = std::make_unique<ofTrueTypeFont>();
	Oranienbaum_big = std::make_unique<ofTrueTypeFont>();
	Oranienbaum->load("Oranienbaum.ttf", 30);
	Oranienbaum_big->load("Oranienbaum.ttf", 60);

	joy_.setup(GLFW_JOYSTICK_1);
}

TitleScene::~TitleScene()
{
	std::cout << "Remove: TitleScene" << std::endl;
}

void TitleScene::update()
{
	if (is_transiting) {
		if (transition_counter < transition_time) {
			transition_counter++;
		}
		else {
			can_change_scene = true;
		}
		return;
	}

	float XAxis = joy_.getAxis(0);
	if (XAxis < -0.5) {
		push_counter++;
		if (push_counter == 1) {
			choice_idx = (choice_idx + 2) % 3;
		}
	}
	else if (0.5 < XAxis) {
		push_counter++;
		if (push_counter == 1) {
			choice_idx = (choice_idx + 1) % 3;
		}
	}
	else
	{
		push_counter = 0;
	}
	if (joy_.isPressed(0)) {
		this->is_transiting = true;
		this->transition_counter = 0;
	}

	switch (choice_idx)
	{
	case 0:
		this->next_scene = Scene::play_game_scene;
		break;
	case 1:
		this->next_scene = Scene::setting_scene;
		break;
	case 2:
		this->next_scene = Scene::quit_game_scene;
		break;
	default:
		break;
	}
}

void TitleScene::draw()
{
	ofVec2f start_pos = ofVec2f(setting_parameter->window_width / 4, setting_parameter->window_height * 3 / 4);
	ofVec2f setting_pos = ofVec2f(setting_parameter->window_width / 2, setting_parameter->window_height * 3 / 4);
	ofVec2f quit_pos = ofVec2f(setting_parameter->window_width * 3 / 4, setting_parameter->window_height * 3 / 4);

	ofSetColor(255, 255, 255);
	ofDrawRectangle(0, 0, setting_parameter->window_width, setting_parameter->window_height);

	ofSetColor(0, 0, 0);
	Oranienbaum_big->drawString("**Insert Title**", 400, 300);
	Oranienbaum->drawString("Start", start_pos.x, start_pos.y);
	Oranienbaum->drawString("Setting", setting_pos.x, setting_pos.y);
	Oranienbaum->drawString("Quit", quit_pos.x, quit_pos.y);

	int size_of_r = 15;
	ofPushMatrix();
	switch (choice_idx)
	{
	case 0:
		ofTranslate(start_pos.x - 30, start_pos.y - size_of_r, 0);
		break;
	case 1:
		ofTranslate(setting_pos.x - 30, setting_pos.y - size_of_r, 0);
		break;
	case 2:
		ofTranslate(quit_pos.x - 30, quit_pos.y - size_of_r, 0);
		break;

	default:
		break;
	}
	ofNoFill();
	ofSetLineWidth(3.0);
	ofDrawCircle(0, 0, size_of_r);
	ofSetLineWidth(1.0);
	ofFill();
	ofDrawTriangle(size_of_r, 0, size_of_r * cos(2 * PI / 3), size_of_r * sin(2 * PI / 3), size_of_r * cos(-2 * PI / 3), size_of_r * sin(-2 * PI / 3));
	ofPopMatrix();
	//-------transition_out-----------
	if (this->is_transiting)
	{
		ofSetColor(255, 255, 255, ofMap(transition_counter, 0, transition_time, 0, 255));
		ofDrawRectangle(0, 0, setting_parameter->window_width, setting_parameter->window_height);
	}
}

void TitleScene::keyPressed(int key) {
	if (this->is_transiting) {
		return;
	}

	switch (key) {
	case OF_KEY_LEFT:
		choice_idx = (choice_idx + 2) % 3;
		break;
	case OF_KEY_RIGHT:
		choice_idx = (choice_idx + 1) % 3;
		break;
	case OF_KEY_RETURN:
		this->is_transiting = true;
		this->transition_counter = 0;
		break;
	}
}

SettingScene::SettingScene(std::unique_ptr<SettingParameter> _setting_parameter)
{
	setting_parameter = std::move(_setting_parameter);

	transition_counter = 0;
	transition_time = 30;
	can_change_scene = false;
	is_transiting = false;
	next_scene = Scene::title_scene;

	choice_idx = 0;
	push_counter = 0;
	Oranienbaum = std::make_unique<ofTrueTypeFont>();
	Oranienbaum->load("Oranienbaum.ttf", 30);

	joy_.setup(GLFW_JOYSTICK_1);
}

SettingScene::~SettingScene()
{
	std::cout << "Remove: SettingScene" << std::endl;
}

void SettingScene::update()
{
	if (is_transiting) {
		if (transition_counter < transition_time) {
			transition_counter++;
		}
		else {
			can_change_scene = true;
		}
		return;
	}

	if (joy_.isPressed(0) && choice_idx == 3) {
		this->is_transiting = true;
		this->transition_counter = 0;
	}

	float YAxis = joy_.getAxis(1);
	float XAxis = joy_.getAxis(0);

	if (YAxis < -0.7) {
		push_counter++;
		if (push_counter == 1 || (60 < push_counter && (push_counter % 10 == 0))) {
			choice_idx = (choice_idx + 3) % 4;
		}
	}
	else if (0.7 < YAxis) {
		push_counter++;
		if (push_counter == 1 || (60 < push_counter && (push_counter % 10 == 0))) {
			choice_idx = (choice_idx + 1) % 4;
		}
	}
	else
	{
		push_counter = 0;
	}

	switch (choice_idx)
	{
	case 0:
		if (XAxis < -0.5 && 0 < setting_parameter->bgm_volume) {
			setting_parameter->bgm_volume -= (joy_.isPushing(0) ? 0.01 : 0.02);
		}
		else if (0.5 < XAxis && setting_parameter->bgm_volume < 1) {
			setting_parameter->bgm_volume += (joy_.isPushing(0) ? 0.01 : 0.02);
		}

		if (setting_parameter->bgm_volume < 0)
		{
			setting_parameter->bgm_volume = 0.0;
		}
		if (1.0 < setting_parameter->bgm_volume)
		{
			setting_parameter->bgm_volume = 1.0;
		}
		break;
	case 1:
		if (XAxis < -0.5 && 0 < setting_parameter->se_volume) {
			setting_parameter->se_volume -= (joy_.isPushing(0) ? 0.01 : 0.02);
		}
		else if (0.5 < XAxis && setting_parameter->se_volume < 1) {
			setting_parameter->se_volume += (joy_.isPushing(0) ? 0.01 : 0.02);
		}

		if (setting_parameter->se_volume < 0)
		{
			setting_parameter->se_volume = 0.0;
		}
		if (1.0 < setting_parameter->se_volume)
		{
			setting_parameter->se_volume = 1.0;
		}
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}
}

void SettingScene::draw()
{
	ofVec2f volume_pos = ofVec2f(setting_parameter->window_width / 8, setting_parameter->window_height * 3 / 10);
	ofVec2f se_pos = ofVec2f(setting_parameter->window_width / 8, setting_parameter->window_height * 4 / 10);
	ofVec2f windowmode_pos = ofVec2f(setting_parameter->window_width / 8, setting_parameter->window_height * 5 / 10);
	ofVec2f return_pos = ofVec2f(setting_parameter->window_width / 2 - 30, setting_parameter->window_height * 5 / 6);

	float bar_left = setting_parameter->window_width * 4 / 8;
	float bar_right = setting_parameter->window_width * 7 / 8;

	ofSetColor(255, 255, 255);
	ofDrawRectangle(0, 0, setting_parameter->window_width, setting_parameter->window_height);

	ofSetColor(0, 0, 0);
	Oranienbaum->drawString("Setting", setting_parameter->window_width / 2 - 30, 50);

	Oranienbaum->drawString("BGM Volume", volume_pos.x, volume_pos.y);
	ofSetLineWidth(2.0);
	ofDrawLine(bar_left, volume_pos.y, bar_right, volume_pos.y);
	ofNoFill();
	ofSetLineWidth(3.0);
	ofDrawCircle(ofMap(setting_parameter->bgm_volume, 0, 1, bar_left, bar_right), volume_pos.y, 15);
	ofFill();
	ofSetLineWidth(1.0);
	ofDrawCircle(ofMap(setting_parameter->bgm_volume, 0, 1, bar_left, bar_right), volume_pos.y, 10);

	Oranienbaum->drawString("SE Volume", se_pos.x, se_pos.y);
	ofSetLineWidth(2.0);
	ofDrawLine(bar_left, se_pos.y, bar_right, se_pos.y);
	ofNoFill();
	ofSetLineWidth(3.0);
	ofDrawCircle(ofMap(setting_parameter->se_volume, 0, 1, bar_left, bar_right), se_pos.y, 15);
	ofFill();
	ofSetLineWidth(1.0);
	ofDrawCircle(ofMap(setting_parameter->se_volume, 0, 1, bar_left, bar_right), se_pos.y, 10);

	Oranienbaum->drawString("Full Screen", windowmode_pos.x, windowmode_pos.y);

	Oranienbaum->drawString("Return", return_pos.x, return_pos.y);

	//----------cursor----------------
	int size_of_cursor_r = 15;
	ofPushMatrix();
	switch (choice_idx)
	{
	case 0:
		ofTranslate(volume_pos.x - 30, volume_pos.y - size_of_cursor_r, 0);
		break;
	case 1:
		ofTranslate(se_pos.x - 30, se_pos.y - size_of_cursor_r, 0);
		break;
	case 2:
		ofTranslate(windowmode_pos.x - 30, windowmode_pos.y - size_of_cursor_r, 0);
		break;
	case 3:
		ofTranslate(return_pos.x - 30, return_pos.y - size_of_cursor_r, 0);
		break;

	default:
		break;
	}
	ofNoFill();
	ofSetLineWidth(3.0);
	ofDrawCircle(0, 0, size_of_cursor_r);
	ofFill();
	ofSetLineWidth(1.0);
	ofDrawTriangle(size_of_cursor_r, 0, size_of_cursor_r * cos(2 * PI / 3), size_of_cursor_r * sin(2 * PI / 3), size_of_cursor_r * cos(-2 * PI / 3), size_of_cursor_r * sin(-2 * PI / 3));
	ofPopMatrix();

	//-------transition_out-----------
	if (this->is_transiting)
	{
		ofSetColor(255, 255, 255, ofMap(transition_counter, 0, transition_time, 0, 255));
		ofDrawRectangle(0, 0, setting_parameter->window_width, setting_parameter->window_height);
	}
}

void SettingScene::keyPressed(int key)
{
	if (this->is_transiting) {
		return;
	}

	switch (key) {
	case OF_KEY_DOWN:
		choice_idx = (choice_idx + 3) % 4;
		break;
	case OF_KEY_UP:
		choice_idx = (choice_idx + 1) % 4;
		break;
	case OF_KEY_RETURN:
		this->is_transiting = true;
		this->transition_counter = 0;
		break;
	}
}