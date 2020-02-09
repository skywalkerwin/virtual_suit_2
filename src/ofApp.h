#pragma once

#include "ofMain.h"

#include "ofxXmlPoco.h"
#include "ofxIO.h"
#include "ofxSerial.h"
#include "Body.h"
#include "Arm.h"
#include "Sword.h"
#include "LightCycle.h"
#include "Rock.h"
#include "Legs.h"
#include "Oracle.h"


class ofApp : public ofBaseApp {

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
	void exit();

	Oracle oracle;
	Body mybody;
	Sword saber;
	//Rock rock;
	ofEasyCam cam;
	LightCycle cycle;
	int z = 0;
	int y = 0;
	int x = 0; 


	int cycleFrames = 60;
	int framecount = 0;
	vec3 stepClockPos;
	float stepClockAngle = 0;
	ofMesh stepClock;

	int cycleFrames2 = 120;
	int framecount2 = 0;
	vec3 stepClockPos2;
	float stepClockAngle2 = 0;
	ofMesh stepClock2;

	bool exclu[5] = { 1, 1, 1, 1, 1 };
	//bool ex1 = 1;
	//bool ex2 = 1;
	//bool ex3 = 1;
	//bool ex4 = 1;
	//bool ex5 = 1;
	//bool exclu[5] = { ex1, ex2, ex3, ex4, ex5 };
	
	//camera stuff
	ofSerial serial;
	ofVideoGrabber vidGrabber;
	ofTexture videoTexture;
	int camWidth;
	int camHeight;

	int res = 5;
	float sphereRad = 200;
	float sphereRotate = 0;
	float sphereHeight = ofGetHeight();
	int sphereDir = -1;
	bool showCam = 1;

};