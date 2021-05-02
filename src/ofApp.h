#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "GameScene.h"
#include <map>
#include <memory>
#include <iostream>

class ofApp : public ofBaseApp {
private:
	std::unique_ptr<Scene> current_scene;
	std::unique_ptr<SettingParameter> setting_parameter = std::make_unique<SettingParameter>();

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void exit() {
		std::cout << "ofApp::exit()\n";
	}
	std::unique_ptr<Scene> sceneFactory(Scene::SceneIdx idx);
};
