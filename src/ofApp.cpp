#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	this->current_scene = sceneFactory(Scene::title_scene);
}

//--------------------------------------------------------------
void ofApp::update() {
	if (this->current_scene->canChangeScene()) {
		setting_parameter.reset();
		setting_parameter = std::move(current_scene->getSettingParameter());
		Scene::SceneIdx next = current_scene->getNextScene();
		this->current_scene.reset();
		this->current_scene = sceneFactory(next);
	}
	this->current_scene->update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	this->current_scene->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	this->current_scene->keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	this->current_scene->keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	this->current_scene->mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
}

std::unique_ptr<Scene> ofApp::sceneFactory(Scene::SceneIdx idx)
{
	switch (idx)
	{
	case Scene::quit_game_scene:
		return std::make_unique<QuitScene>(std::move(setting_parameter));
		break;
	case Scene::title_scene:
		return std::make_unique<TitleScene>(std::move(setting_parameter));
		break;
	case Scene::play_game_scene:
		return std::make_unique<GameScene>(std::move(setting_parameter));
		break;
	case Scene::setting_scene:
		return std::make_unique<SettingScene>(std::move(setting_parameter));
		break;
	default:
		break;
	}
	return std::make_unique<QuitScene>(std::move(setting_parameter));
}
