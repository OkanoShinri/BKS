#pragma once
#include "ofMain.h"
#include "ofxJoystick.h"
#include <iostream>
#include <memory>
#include <list>

class Scene {
public:
	enum SceneIdx { quit_game_scene = 0, title_scene, play_game_scene };

	virtual ~Scene() {};
	virtual void update() {};
	virtual void draw() {};
	virtual void keyPressed(int key) {};
	virtual void keyReleased(int key) {};
	virtual void mouseMoved(int x, int y) {};
	virtual void mouseDragged(int x, int y, int button) {};
	virtual void mousePressed(int x, int y, int button) {};
	virtual void mouseReleased(int x, int y, int button) {};
	virtual void windowResized(int w, int h) {};
	virtual void feadout(int mun) {};

	int transition_counter = 0;
	int transition_time = 0;
	bool can_change_scene = false;
	bool is_transiting = false;
	SceneIdx nextScene = title_scene;
};

class QuitScene :public Scene {
private:
	void update() { std::exit(0); };
	void draw() {};
};

class TitleScene :public Scene {
public:
	TitleScene();
	~TitleScene();
private:
	void update();
	void draw();
	void keyPressed(int key);
	void feadout(int mun) {};

	enum TitleChoiceIdx { play = 0, quit };
	TitleChoiceIdx choice;

	ofTrueTypeFont verdana;
	ofxJoystick joy_;
};
