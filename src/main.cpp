#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
	//ofSetupOpenGL(1920, 1080, OF_FULLSCREEN);
	ofSetupOpenGL(1280, 720, OF_WINDOW);

	ofSetVerticalSync(false);
	ofSetFrameRate(60);

	ofRunApp(new ofApp());
}