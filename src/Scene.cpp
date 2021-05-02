#include "Scene.h"

QuitScene::QuitScene(std::unique_ptr<SettingParameter> _setting_parameter)
{
	setting_parameter = std::move(_setting_parameter);
}

TitleScene::TitleScene(std::unique_ptr<SettingParameter> _setting_parameter)
{
	setting_parameter = std::move(_setting_parameter);

	this->nextScene = Scene::play_game_scene;
	this->choice = TitleScene::play;
	this->can_change_scene = false;
	this->is_transiting = false;
	this->verdana.load("verdana.ttf", 50);
	this->transition_counter = 0;

	joy_.setup(GLFW_JOYSTICK_1);
}

TitleScene::~TitleScene()
{
	std::cout << "Remove: TitleScene" << std::endl;
}

void TitleScene::update()
{
	if (this->is_transiting) {
		if (this->transition_counter + 10 > 255)
		{
			this->transition_counter = 255;
			this->can_change_scene = true;
		}
		else {
			this->transition_counter += 10;
		}
		return;
	}

	if (joy_.isPressed(0)) {
		this->is_transiting = true;
		this->transition_counter = 0;
	}
	float XAxis = joy_.getAxis(0);
	if (XAxis < -0.5) {
		this->choice = TitleScene::play;
	}
	else if (0.5 < XAxis) {
		this->choice = TitleScene::quit;
	}

	switch (choice)
	{
	case TitleScene::play:
		this->nextScene = Scene::play_game_scene;
		break;
	case TitleScene::quit:
		this->nextScene = Scene::quit_game_scene;
		break;
	default:
		break;
	}
}

void TitleScene::draw()
{
	float start_x = ofGetWidth() / 4,
		start_y = ofGetHeight() * 3 / 4,
		quit_x = ofGetWidth() * 3 / 4,
		quit_y = ofGetHeight() * 3 / 4;

	ofSetColor(255, 255, 255);
	this->verdana.drawString("START", start_x, start_y);
	this->verdana.drawString("QUIT", quit_x, quit_y);

	switch (this->choice)
	{
	case TitleScene::play:
		ofDrawTriangle(start_x - 50, start_y - 30, start_x - 50, start_y + 30, start_x - 10, start_y);
		break;
	case TitleScene::quit:
		ofDrawTriangle(quit_x - 50, quit_y - 30, quit_x - 50, quit_y + 30, quit_x - 10, quit_y);
		break;
	default:
		break;
	}

	if (this->is_transiting) {
		ofSetColor(255, 255, 255, transition_counter);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	}
}
void TitleScene::keyPressed(int key) {
	if (this->is_transiting) {
		return;
	}

	switch (key) {
	case OF_KEY_LEFT:
		this->choice = TitleScene::play;
		break;
	case OF_KEY_RIGHT:
		this->choice = TitleScene::quit;
		break;
	case OF_KEY_RETURN:
		this->is_transiting = true;
		this->transition_counter = 0;
		break;
	}
}

