#pragma once
#include "ofMain.h"
#include "ofxJoystick.h"
#include "SettingParameter.h"
#include <iostream>
#include <memory>
#include <list>

class Scene {
public:
	enum SceneIdx { quit_game_scene = 0, title_scene, play_game_scene };

	virtual ~Scene() {};
	virtual std::unique_ptr<SettingParameter> getSettingParameter() = 0;
	virtual void draw() {};
	virtual void feadout(int mun) {};
	virtual void keyPressed(int key) {};
	virtual void keyReleased(int key) {};
	virtual void mouseDragged(int x, int y, int button) {};
	virtual void mouseMoved(int x, int y) {};
	virtual void mousePressed(int x, int y, int button) {};
	virtual void mouseReleased(int x, int y, int button) {};
	virtual void update() {};
	virtual void windowResized(int w, int h) {};

	int transition_counter = 0;
	int transition_time = 0;
	bool can_change_scene = false;
	bool is_transiting = false;
	SceneIdx nextScene = title_scene;
};

class QuitScene :public Scene {
public:
	QuitScene(std::unique_ptr<SettingParameter>&& _setting_parameter);
	void update() { std::exit(0); };
	void draw() {};
	std::unique_ptr<SettingParameter> getSettingParameter() {
		return std::move(setting_parameter);
	}
private:
	std::unique_ptr<SettingParameter> setting_parameter;
};

class TitleScene :public Scene {
public:
	TitleScene(std::unique_ptr<SettingParameter>&& _setting_parameter);
	~TitleScene();
	std::unique_ptr<SettingParameter> getSettingParameter() {
		return std::move(setting_parameter);
	}
private:
	void update();
	void draw();
	void keyPressed(int key);
	void feadout(int mun) {};

	enum TitleChoiceIdx { play = 0, quit };
	ofTrueTypeFont verdana;
	ofxJoystick joy_;
	std::unique_ptr<SettingParameter> setting_parameter;
	TitleChoiceIdx choice;
};
