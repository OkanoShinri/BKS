#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
	ofGLFWWindowSettings settings;
	settings.setSize(1280, 720);
	settings.resizable = false;
	ofCreateWindow(settings);

	//ofSetupOpenGL(1920, 1080, OF_FULLSCREEN);
	//ofSetupOpenGL(1280, 720, OF_WINDOW);

	//ofSetVerticalSync(false);
	//ofSetFrameRate(60);
	ofSetVerticalSync(true);

	ofRunApp(new ofApp());
}