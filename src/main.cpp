#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
	ofGLFWWindowSettings settings;
	settings.setSize(1280, 720);
	settings.resizable = false;
	ofCreateWindow(settings);

	ofSetVerticalSync(true);

	ofRunApp(new ofApp());
}
